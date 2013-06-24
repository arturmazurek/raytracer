//
//  Renderer.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Renderer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <thread>

#include "BaseLight.h"
#include "BaseObject.h"
#include "Bitmap.h"
#include "Log.h"
#include "Math.h"
#include "Matrix.h"
#include "PointLight.h"
#include "SphereLight.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"

static const double DEFAULT_FOV = 0.4 * Math::PI;
static const int DEFAULT_SUPERSAMPLING = 1;
static const double DEFAULT_RAY_BIAS = 0.001;

static inline FloatType uniRand() {
    return (FloatType)(rand() % 1000000) / 1000000;
}

static inline Vector onSphereRand() {    
    FloatType theta = 2 * Math::PI * uniRand();
    FloatType phi = acos(2*uniRand() - 1);
    
    return {sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)};
}

static inline Vector hemisphereRand(const Vector& normal) {
    Vector result = onSphereRand();
    FloatType k = dot(result, normal);
    if(k < 0) {
        result -= 2 * k * normal;
    }
    
    return result;
}

const int Renderer::BLOCK_DEFAULT_W{20};
const int Renderer::BLOCK_DEFAULT_H{20};

Renderer::Renderer() : m_width{0}, m_height{0}, m_fovY{DEFAULT_FOV}, m_superSampling{DEFAULT_SUPERSAMPLING},
m_flipY{false}, m_rayBias{DEFAULT_RAY_BIAS}, m_exposure{1}, m_gamma{1}, m_highestIntensity{0}, m_bouncedRays{0},
m_maxRayDepth{0} {
    srand(static_cast<int>(time(nullptr)));
}

Renderer::~Renderer() {
    
}

void Renderer::setDimensions(int width, int height) {
    assert(width >= 0);
    assert(height >= 0);
    
    m_width = width;
    m_height = height;
}

int Renderer::width() const {
    return m_width;
}

int Renderer::height() const {
    return m_height;
}

void Renderer::setFovY(double fovY) {
    assert(fovY >= 0);
    assert(fovY < Math::PI);
    
    m_fovY = fovY;
}

void Renderer::setSuperSampling(int amount) {
    assert(amount >= 1);
    m_superSampling = amount;
}

int Renderer::getSuperSampling() const {
    return m_superSampling;
}

void Renderer::setFlipY(bool value) {
    m_flipY = value;
}

bool Renderer::getFlipY() const {
    return m_flipY;
}

void Renderer::setRayBias(double bias) {
    assert(bias >= 0);
    m_rayBias = bias;
}

double Renderer::rayBias() const {
    return m_rayBias;
}

void Renderer::setExposure(double exposure) {
    m_exposure = exposure;
}

double Renderer::exposure() const {
    return m_exposure;
}

void Renderer::setGamma(double gamma) {
    m_gamma = gamma;
}

double Renderer::gamma() const {
    return m_gamma;
}

void Renderer::setBouncedRays(int count) {
    m_bouncedRays = count;
}

int Renderer::bouncedRays() const {
    return m_bouncedRays;
}

void Renderer::setMaxRayDepth(int count) {
    m_maxRayDepth = count;
}

int Renderer::maxRayDepth() const {
    return m_maxRayDepth;
}

Color Renderer::tracePath(const Scene& s, const Ray& r) const {
    if(r.depth > m_maxRayDepth) {
        return {};
    }
    
    Vector intersection;
    Vector normal;
    BaseObject* obj = s.findIntersection(r, intersection, normal);
    
    if(!obj) {
        return {};
    }
    
    Vector biasedPos = intersection + m_rayBias*normal;
    Color reflected;
    
    Ray newRay{biasedPos, hemisphereRand(normal), r.depth + 1};
    
    FloatType cosTheta = dot(newRay.direction, normal);
    Color BDRF = obj->material()->reflectance() * Color{cosTheta, cosTheta, cosTheta, 1};
    BDRF *= obj->material()->color();
    
    reflected = tracePath(s, newRay);
    return obj->material()->emmitance() + Color{BDRF * reflected};
}

void Renderer::renderScene(Scene& s, std::function<void(const Bitmap&, int)> callback) {
    prepareRender(s);
    pathTraceScene(s, callback, 100000);
}

void Renderer::pathTraceScene(Scene& s, std::function<void(const Bitmap&, int)> callback, int iterations) {
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    
    auto colorBuffer = std::unique_ptr<Color[]>{new Color[w * h]};
    auto result = std::unique_ptr<Bitmap>{new Bitmap{m_width, m_height}};
    
    auto doneBlocks = prepareBlocks();
    decltype(doneBlocks) blocks;
    
    std::mutex blocksMutex;
    
    auto pathWorker = [&]() {
        while(true) {
            Block b;
            blocksMutex.lock();
            if(blocks.empty()) {
                blocksMutex.unlock();
                break;
            }
            b = blocks.front();
            blocks.pop_front();
            blocksMutex.unlock();
            
            int count = 30;
            pathTracing(s, colorBuffer.get(), b, count, b.iterations);
            
            b.iterations += count;
            
            blocksMutex.lock();
            doneBlocks.push_back(b);
            blocksMutex.unlock();
        }
    };
    
    auto tempBuffer = std::unique_ptr<Color[]>{new Color[w * h]};
    
    while(true) {
        int percent = 100 - 100 * (iterations - doneBlocks.front().iterations) / iterations;
        
        memcpy(tempBuffer.get(), colorBuffer.get(), w * h * sizeof(Color));
        auto notifyBlocks = doneBlocks;
        
        std::thread notifyThread{[&]() {
            for(const auto& b : notifyBlocks) {
                processImage(tempBuffer.get(), b, [this](Color& c){
                    correctExposure(c);
                    correctGamma(c);
                });
                
                scaleDown(tempBuffer.get(), *result, b, true);
            }
            
            callback(*result, std::min(100, percent));
        }};
    
        if(percent >= 100) {
            if(notifyThread.joinable()) {
                notifyThread.join();
            }
            break;
        }
        
        blocks = doneBlocks;
        doneBlocks.clear();
        
        processParallel(pathWorker);
        if(notifyThread.joinable()) {
            notifyThread.join();
        }
    }
}

void Renderer::pathTracing(const Scene& s, Color* result, const Block& block, int pixelIters, int total) {
    for(int iter = 1; iter <= pixelIters; ++iter) {
        for(int j = block.y; j < block.y + block.h; ++j) {
            for(int i = block.x; i < block.x + block.w; ++i) {
                Ray r = m_camera.viewPointToRay((double)i/m_superSampling - 0.5*m_width, (double)j/m_superSampling - 0.5*m_height);
                
                Color& resultingColor = result[j*block.totalW + i];
                resultingColor = resultingColor * (total + iter - 1) / (total + iter);
                resultingColor += tracePath(s, r) / (total + iter);
            }
        }
    }
}

void Renderer::processParallel(std::function<void()> worker) const {
    unsigned threadsCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    for(int i = 0; i < threadsCount; ++i) {
        threads.push_back(std::thread{worker});
    }
    
    for(auto& thread : threads) {
        thread.join();
    }
}

std::deque<Renderer::Block> Renderer::prepareBlocks(int width, int height) const {
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    
    int blockW = width * m_superSampling;
    int blockH = height * m_superSampling;
    
    std::vector<Block> result;
    
    for(int j = 0; j < h; j += blockH) {
        for(int i = 0; i < w; i += blockW) {
            result.push_back({i, j, std::min(blockW, w - i), std::min(blockH, h - j), w, h, 0});
        }
    }
    
    std::random_shuffle(begin(result), end(result), [](int max){return rand() % max;});
    
    return std::deque<Renderer::Block>(begin(result), end(result));
}

void Renderer::scaleDown(Color* fromBuffer, Bitmap& toBitmap, const Block& b, bool overwrite) const {
    int x = b.x / m_superSampling;
    int y = b.y / m_superSampling;
    int w = b.w / m_superSampling;
    int h = b.h / m_superSampling;
    
    for(int j = y; j < y + h; ++j) {
        for(int i = x; i < x + w; ++i) {
            Color c;
            for(int xs = 0; xs < m_superSampling; ++xs) {
                for(int ys = 0; ys < m_superSampling; ++ys) {
                    size_t index = (j*m_superSampling + ys)*m_width*m_superSampling + i*m_superSampling+xs;
                    c += fromBuffer[index];
                }
            }
            
            c /= m_superSampling * m_superSampling;
            
            correctExposure(c);
            
            if(m_flipY) {
                toBitmap.pixel(i, m_height - j - 1) = c;
            } else {
                toBitmap.pixel(i, j) = c;
            }
        }
    }
}

void Renderer::prepareRender(Scene& s) {
    double focalLength = m_height / (2 * std::tan(0.5 * m_fovY));
    m_camera.setFocalLength(focalLength);
    m_highestIntensity = std::numeric_limits<double>::min();
    
    s.prepare();
}

void Renderer::processImage(Color* bitmap, const Block& block, std::function<void(Color&)> filter) const {
    for(int j = block.y; j < block.y + block.h; ++j) {
        for(int i = block.x; i < block.x + block.w; ++i) {
            filter(bitmap[j*block.totalW + i]);
        }
    }
}

void Renderer::correctGamma(Color& c) const {
    c.r = pow(c.r, m_gamma);
    c.g = pow(c.g, m_gamma);
    c.b = pow(c.b, m_gamma);
}

void Renderer::correctExposure(Color& c) const {
    c.r = 1.0 - exp(-c.r * m_exposure);
    c.g = 1.0 - exp(-c.g * m_exposure);
    c.b = 1.0 - exp(-c.b * m_exposure);
}

