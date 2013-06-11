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
#include <limits>
#include <memory>
#include <string>

#include "BaseLight.h"
#include "BaseObject.h"
#include "Bitmap.h"
#include "Log.h"
#include "Math.h"
#include "Matrix.h"
#include "Ray.h"
#include "Scene.h"
#include "Vector.h"

static const double DEFAULT_FOV = 0.4 * Math::PI;
static const int DEFAULT_SUPERSAMPLING = 1;
static const double DEFAULT_RAY_BIAS = 0.001;

static const int BLOCK_BASE_W = 20;
static const int BLOCK_BASE_H = 20;

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

void Renderer::renderScene(const Scene& s, std::function<void(const Bitmap&, int)> callback) {
    using namespace std;
    prepareRender();
    
    Vector intersection;
    
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    auto tempBuffer = std::unique_ptr<Color[]>{new Color[w * h]};
    auto result = std::unique_ptr<Bitmap>{new Bitmap{m_width, m_height}};
    
    auto blocks = prepareBlocks();
    double count = blocks.size();
    double index = 0;
    for(const auto& block : blocks) {
        ++index;
        raycast(s, tempBuffer.get(), block);
        
        processImage(tempBuffer.get(), block, [this](Color& c){ this->correctExposure(c); });
        processImage(tempBuffer.get(), block, [this](Color& c){ this->correctGamma(c); });
        
        scaleDown(tempBuffer.get(), *result, block);
        
        callback(*result, static_cast<int>(index / count * 100 + 0.5));
    }
}

std::list<Renderer::Block> Renderer::prepareBlocks() const {
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    
    int blockW = BLOCK_BASE_W * m_superSampling;
    int blockH = BLOCK_BASE_H * m_superSampling;
    
    std::vector<Block> result;
    
    for(int j = 0; j < h; j += blockH) {
        for(int i = 0; i < w; i += blockW) {
            result.push_back({i, j, std::min(blockW, w - i), std::min(blockH, h - j), w, h});
        }
    }
    
    std::random_shuffle(begin(result), end(result), [](int max){return rand() % max;});
    
    return std::list<Renderer::Block>(begin(result), end(result));
}

void Renderer::scaleDown(Color* fromBuffer, Bitmap& toBitmap, const Block& b) const {
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
        
        if(r.depth <= m_maxRayDepth) {
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
        FloatType u = (FloatType)(rand() % 10000) / 10000;
        FloatType v = (FloatType)(rand() % 10000) / 10000;
        
        FloatType theta = 2 * Math::PI * u;
        FloatType phi = acos(2*v - 1);
        
        Vector newNormal{sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)};
        FloatType k = dot(newNormal, normal);
        if(k < 0) {
            newNormal -= 2 * k * normal;
        }
        
        result[i] = {pos, newNormal};
    }
    
    return result;
}

void Renderer::prepareRender() {
    double focalLength = m_height / (2 * std::tan(0.5 * m_fovY));
    m_camera.setFocalLength(focalLength);
    m_highestIntensity = std::numeric_limits<double>::min();
}

Color Renderer::getDiffuse(const Scene& s, const Vector& pos, const Vector& normal) {
    FloatType intensity = 0.075;
    for(auto it = s.lightsBegin(); it != s.lightsEnd(); ++it) {
        Vector biasedPos = pos + m_rayBias*normal;
        Vector toLight = (*it)->position() - biasedPos;
        Ray toLightRay{biasedPos, toLight};
        
        Vector intersection;
        Vector intersectionNormal;
        BaseObject* intersecting = s.findIntersection(toLightRay, intersection, intersectionNormal);
        if(intersecting) {
            if((intersection - biasedPos).lengthSqr() < ((*it)->position() - biasedPos).lengthSqr()) {
                continue;
            }
        }
        
        
        intensity += (*it)->intensityAtPosition(pos, normal);
    }
    
    return {intensity, intensity, intensity, 1};
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

