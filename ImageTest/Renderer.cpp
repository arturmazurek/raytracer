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
#include "Ray.h"
#include "Scene.h"
#include "Vector.h"

static const double DEFAULT_FOV = 0.4 * Math::PI;
static const int DEFAULT_SUPERSAMPLING = 1;
static const double DEFAULT_RAY_BIAS = 0.001;

Renderer::Renderer() : m_width{0}, m_height{0}, m_fovY{DEFAULT_FOV}, m_superSampling{DEFAULT_SUPERSAMPLING},
m_flipY{false}, m_rayBias{DEFAULT_RAY_BIAS}, m_exposure{1}, m_gamma{1}, m_highestIntensity{0}, m_occlusionRays{0},
m_occlusionRaysBounces{0} {
    
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

void Renderer::setOcclusionRays(int count) {
    m_occlusionRays = count;
}

int Renderer::occlusionRays() const {
    return m_occlusionRays;
}

void Renderer::setOcclusionRaysBounces(int count) {
    m_occlusionRaysBounces = count;
}

int Renderer::occlusionRaysBounces() const {
    return m_occlusionRaysBounces;
}

std::unique_ptr<Bitmap> Renderer::renderScene(const Scene& s) {
    using namespace std;
    prepareRender();
    
    Vector intersection;
    
    auto tempBuffer = std::unique_ptr<Color[]>(new Color[m_superSampling * m_width * m_superSampling * m_height]);
    
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Ray r = m_camera.viewPointToRay((double)i/m_superSampling - 0.5*m_width, (double)j/m_superSampling - 0.5*m_height);
            Color c = processRay(s, r);
            
            double magn = c.magnitudeSqr();
            if(magn > m_highestIntensity) {
                m_highestIntensity = magn;
            }
            
            tempBuffer[j*w + i] = c;
        }
    }
    
    processExposure(tempBuffer.get(), w, h);
    
    auto b = std::unique_ptr<Bitmap>(new Bitmap(m_width, m_height));
    for(int j = 0; j < m_height; ++j) {
        for(int i = 0; i < m_width; ++i) {
            Color c;
            for(int xs = 0; xs < m_superSampling; ++xs) {
                for(int ys = 0; ys < m_superSampling; ++ys) {
                    size_t index = (j*m_superSampling + ys)*w + i*m_superSampling+xs;
                    c += tempBuffer[index];
                }
            }
            
            c /= m_superSampling * m_superSampling;
            
            if(m_flipY) {
                b->pixel(i, m_height - j - 1) = c;
            } else {
                b->pixel(i, j) = c;
            }
        }
    }
    
    return b;
}

Color Renderer::processRay(const Scene& s, const Ray& r) {
    Vector intersection;
    Vector normal;
    
    if(s.findIntersection(r, intersection, normal)) {
        Color c = getDiffuse(s, intersection, normal);
        
        return c;
    } else {
        return {};
    }
}

void Renderer::prepareRender() {
    double focalLength = m_height / (2 * std::tan(0.5 * m_fovY));
    m_camera.setFocalLength(focalLength);
    m_highestIntensity = std::numeric_limits<double>::min();
}

Color Renderer::getDiffuse(const Scene& s, const Vector& pos, const Vector& normal) {
    double intensity = 0;
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

void Renderer::processExposure(Color* buffer, int w, int h) const {
    using namespace std;
    
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Color& c = buffer[j*w + i];
            
            c.r = 1.0 - exp(-c.r * m_exposure);
            c.g = 1.0 - exp(-c.g * m_exposure);
            c.b = 1.0 - exp(-c.b * m_exposure);
        }
    }
}
