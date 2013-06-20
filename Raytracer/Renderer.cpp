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
    if(obj->emits) {
        return {10, 10, 10, 1};
    }
    
    Vector biasedPos = intersection + m_rayBias*normal;
    Color reflected;
    
//    if(r.depth + 1 < m_maxRayDepth) {
        Ray newRay{biasedPos, onSphereRand(), r.depth + 1};
        FloatType k = dot(newRay.direction, normal);
        if(k < 0) {
            newRay.direction -= 2 * k * normal;
        }
        
        FloatType cosTheta = dot(newRay.direction, normal);
        Color BDRF = {cosTheta, cosTheta, cosTheta, 1};
        BDRF *= 2;
        if(((long int)obj >> 4) % 2 == 0) {
            BDRF *= Color{1, 0, 0, 1};
        }
        
        reflected = tracePath(s, newRay);
        // Apply the Rendering Equation here.
        return {BDRF * reflected};
//    } else {
//        // shoot straight to light
//        // assume just one light
//        Sphere* emiter = static_cast<Sphere*>(s.allEmiters()[0]);
//        Vector toCenter = emiter->center() - biasedPos;
//        Vector baseA = perpendicular(toCenter);
//        Vector baseB = perpendicular(toCenter, baseA);
//        Vector diff = (2*uniRand() - 1) * baseA + (2*uniRand() -1) * baseB;
//        if(diff.lengthSqr() == 0) {
//            return {};
//        }
//        diff.normalize();
//        diff *= emiter->radius();
//
//        FloatType k = dot(diff, toCenter);
//        if(k > 0) {
//            diff -= 2*k*normalized(toCenter);
//        }
//        
//        Vector dir = (toCenter + diff).normalize();
//        Ray toLight{biasedPos, dir};
//        Vector lastNormal;
//        obj = s.findIntersection(toLight, intersection, lastNormal);
//        if(obj == emiter) {
//            Color result{10, 10, 10, 1};
//            result /= (intersection - biasedPos).lengthSqr();
//            return result;
//        } else {
//            return {};
//        }
//    }
}

Color Renderer::traceBiPath(const Scene& s, const Ray& lightRay, const Ray& eyeRay) const {
    
    Vector lightIntersection, lightNormal;
    BaseObject* lightHit = s.findIntersection(lightRay, lightIntersection, lightNormal);
    if(!lightHit) {
        return {};
    }
    
    Vector eyeIntersection, eyeNormal;
    BaseObject* eyeHit = s.findIntersection(eyeRay, eyeIntersection, eyeNormal);
    if(!eyeHit) {
        return {};
    }
    
    int nextDepth = eyeRay.depth + 1;
    
    if(nextDepth == m_maxRayDepth) {
        // attach intersection points
        Vector connectionIntersection, connectionNormal;
        Ray connectionRay{eyeIntersection + m_rayBias*eyeNormal, (eyeIntersection - lightIntersection).normalize()};
        BaseObject* connectionHit = s.findIntersection(connectionRay, connectionIntersection, connectionNormal);
        if(connectionHit) {
            return {};
        } else {
            FloatType d2 = (connectionIntersection - eyeIntersection).lengthSqr();
            Color result{1000, 1000, 1000, 1};
            result /= d2;
            return result;
        }
    } else {
        // shoot more rays
        Vector newNormal = onSphereRand();
        FloatType k = dot(newNormal, lightNormal);
        if(k < 0) {
            newNormal += 2*k*lightNormal;
        }
        Ray newLightRay{lightIntersection + lightNormal*m_rayBias, newNormal, nextDepth};
        
        newNormal = onSphereRand();
        k = dot(newNormal, eyeNormal);
        if(k < 0) {
            newNormal += 2*k*eyeNormal;
        }
        Ray newEyeRay{eyeIntersection + eyeNormal*m_rayBias, newNormal, nextDepth};
        return traceBiPath(s, newLightRay, newEyeRay);
    }
}

void Renderer::renderScene(Scene& s, std::function<void(const Bitmap&, int)> callback) {
    prepareRender(s);
    pathTraceScene(s, callback, 10000);
}

void Renderer::raytraceScene(Scene& s, std::function<void(const Bitmap&, int)> callback) {
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    auto colorBuffer = std::unique_ptr<Color[]>{new Color[w * h]};
    auto result = std::unique_ptr<Bitmap>{new Bitmap{m_width, m_height}};
    
    auto blocks = prepareBlocks();
    const double initialCount = blocks.size();
    double left = initialCount;
    
    std::mutex blocksLock;
    std::mutex counterLock;
    
    auto worker = [&]() {
        while(true) {
            Block block;
            blocksLock.lock();
            if(blocks.size()) {
                block = blocks.front();
                blocks.pop_front();
                blocksLock.unlock();
            } else {
                blocksLock.unlock();
                return;
            }
            
            raycast(s, colorBuffer.get(), block);
            
            processImage(colorBuffer.get(), block, [this](Color& c){
                correctExposure(c);
                correctGamma(c);
            });
            
            scaleDown(colorBuffer.get(), *result, block);
            
            counterLock.lock();
            --left;
            counterLock.unlock();
        }
    };
    
    auto notifier = [&]() {
        const std::chrono::milliseconds sleepDuration{200};
        
        while(true) {
            int localLeft;
            counterLock.lock();
            localLeft = left;
            counterLock.unlock();
            
            callback(*result, static_cast<int>((initialCount - localLeft) / initialCount * 100));
            if(localLeft == 0) {
                break;
            }
            
            std::this_thread::sleep_for(sleepDuration);
        };
    };
    std::thread notifierThread(notifier);
    
    processParallel(worker);
    notifierThread.join();
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
 
void Renderer::raycast(const Scene& s, Color* result, const Block& block) {
    for(int j = block.y; j < block.y + block.h; ++j) {
        for(int i = block.x; i < block.x + block.w; ++i) {
            Ray r = m_camera.viewPointToRay((double)i/m_superSampling - 0.5*m_width, (double)j/m_superSampling - 0.5*m_height);
            result[j*block.totalW + i] = processRay(s, r);
        }
    }
}

Color Renderer::processRay(const Scene& s, const Ray& r) {
    Vector intersection;
    Vector normal;
    
    if(s.findIntersection(r, intersection, normal)) {
        Color c = getDiffuse(s, intersection, normal);
        
        if(r.depth < m_maxRayDepth) {
            auto newRays = createBouncedRays(intersection, normal, m_bouncedRays);
            Color bounced{0, 0, 0, 1};
            for(int i = 0; i < m_bouncedRays; ++i) {
                newRays[i].depth = r.depth + 1;
                bounced += processRay(s, newRays[i]) * static_cast<Color::ValueType>(dot(normal, newRays[i].direction));
            }
            c += bounced / m_bouncedRays;
        }
        
        return c;
    } else {
        return {};
    }
}

std::unique_ptr<Ray[]> Renderer::createBouncedRays(const Vector& intersection, const Vector& normal, int count) const {
    using namespace std;
    
    auto result = std::unique_ptr<Ray[]>{new Ray[count]};
    const Vector pos = intersection + normal * m_rayBias;
    
    for(int i = 0; i < count; ++i) {
        Vector newNormal = onSphereRand();
        FloatType k = dot(newNormal, normal);
        if(k < 0) {
            newNormal -= 2 * k * normal;
        }
        
        result[i] = {pos, newNormal};
    }
    
    return result;
}

void Renderer::prepareRender(Scene& s) {
    double focalLength = m_height / (2 * std::tan(0.5 * m_fovY));
    m_camera.setFocalLength(focalLength);
    m_highestIntensity = std::numeric_limits<double>::min();
    
    s.prepare();
}

Color Renderer::getDiffuse(const Scene& s, const Vector& pos, const Vector& normal) {
    FloatType intensity = 0.075;
    for(auto it = s.lightsBegin(); it != s.lightsEnd(); ++it) {
        Vector biasedPos = pos + m_rayBias*normal;
        
        switch ((*it)->type()) {
            case BaseLight::Type::TYPE_POINT_LIGHT:
                intensity += handlePointLight(s, *static_cast<PointLight*>((*it).get()), biasedPos, normal);
                break;
                
            case BaseLight::Type::TYPE_SPHERE_LIGHT:
                intensity += handleSphereLight(s, *static_cast<SphereLight*>((*it).get()), biasedPos, normal);
                break;
                
            default:
                assert(!"Unsupported light type");
                break;
        }
    }
    
    return {intensity, intensity, intensity, 1};
}

FloatType Renderer::handlePointLight(const Scene& s, const PointLight& light, const Vector& pos, const Vector& normal) const {
    using namespace std;
    
    Vector toLight = light.position() - pos;
    toLight.normalize();
    Ray ray{pos, toLight};
    
    Vector intersection;
    Vector intersectionNormal;
    BaseObject* intersecting = s.findIntersection(ray, intersection, intersectionNormal);
    if(intersecting && (intersection - pos).lengthSqr() < (light.position() - pos).lengthSqr()) {
        return 0;
    } else {
        return max(dot(normal, toLight), 0);
    }
}

FloatType Renderer::handleSphereLight(const Scene& s, const SphereLight& light, const Vector& pos, const Vector& normal) const {
    using namespace std;
    
    Vector toCenter = light.center() - pos;
    
    FloatType result = 0;
    int samples = m_bouncedRays;
    Vector intersection;
    Vector intersectionNormal;
    
    for(int i = 0; i < samples; ++i) {
        Vector sampleDir = onSphereRand() * light.radius() + toCenter;
        sampleDir.normalize();
        BaseObject* intersecting = s.findIntersection({pos, sampleDir}, intersection, intersectionNormal);
        // TODO: this one doesn't handle cases well some of the cases
        // make the light's sphere actually an object in the scene
        if(intersecting && (intersection - pos).lengthSqr() < (light.center() - pos).lengthSqr()) {
            continue;
        } else {
            result += max(dot(normal, sampleDir), 0);
        }
    }
    
    return result / samples;
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

