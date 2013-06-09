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

Renderer::Renderer() : m_width{0}, m_height{0}, m_fovY{DEFAULT_FOV}, m_superSampling{DEFAULT_SUPERSAMPLING},
m_flipY{false}, m_rayBias{DEFAULT_RAY_BIAS}, m_exposure{1}, m_gamma{1}, m_highestIntensity{0}, m_bouncedRays{0},
m_maxRayDepth{0} {
    
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

std::unique_ptr<Bitmap> Renderer::renderScene(const Scene& s) {
    using namespace std;
    prepareRender();
    
    Vector intersection;
    
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    auto tempBuffer = std::unique_ptr<Color[]>{new Color[w * h]};
    
    raycast(s, tempBuffer.get(), w, h);
    processExposure(tempBuffer.get(), w, h);
    auto result = scaleDown(std::move(tempBuffer));
    correctGamma(*result, m_width, m_height);
    
    return std::move(result);
}

std::unique_ptr<Bitmap> Renderer::scaleDown(std::unique_ptr<Color[]> rawBuffer) const {
    auto result = std::unique_ptr<Bitmap>{new Bitmap(m_width, m_height)};
    
    for(int j = 0; j < m_height; ++j) {
        for(int i = 0; i < m_width; ++i) {
            Color c;
            for(int xs = 0; xs < m_superSampling; ++xs) {
                for(int ys = 0; ys < m_superSampling; ++ys) {
                    size_t index = (j*m_superSampling + ys)*m_width*m_superSampling + i*m_superSampling+xs;
                    c += rawBuffer[index];
                }
            }
            
            c /= m_superSampling * m_superSampling;
            
            if(m_flipY) {
                result->pixel(i, m_height - j - 1) = c;
            } else {
                result->pixel(i, j) = c;
            }
        }
    }
    
    return std::move(result);
}
 
void Renderer::raycast(const Scene& s, Color* result, int width, int height) {
    for(int j = 0; j < height; ++j) {
        for(int i = 0; i < width; ++i) {
            Ray r = m_camera.viewPointToRay((double)i/m_superSampling - 0.5*m_width, (double)j/m_superSampling - 0.5*m_height);
            Color c = processRay(s, r);
            
            double magn = c.magnitudeSqr();
            if(magn > m_highestIntensity) {
                m_highestIntensity = magn;
            }
            
            result[j*width + i] = c;
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

void Renderer::correctGamma(Bitmap& b, int w, int h) const {
    using namespace std;
    
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Bitmap::PixelInfo& pixel = b.pixel(i, j);
            pixel.r = pow(static_cast<double>(pixel.r) / 255, m_gamma) * 255;
            pixel.g = pow(static_cast<double>(pixel.g) / 255, m_gamma) * 255;
            pixel.b = pow(static_cast<double>(pixel.b) / 255, m_gamma) * 255;
        }
    }
}
