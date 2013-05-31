//
//  Renderer.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Renderer.h"

#include <cassert>
#include <cmath>
#include <memory>

#include "Bitmap.h"
#include "Math.h"
#include "Ray.h"
#include "Scene.h"
#include "Vector.h"

Renderer::Renderer() : m_width{0}, m_height{0}, m_fovY{(double)(0.5 * Math::PI)}, m_superSampling{1} {
    
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

std::unique_ptr<Bitmap> Renderer::renderScene(const Scene& s) {
    prepareRender();
    
    Vector intersection;
    
    auto tempBuffer = std::unique_ptr<Color[]>(new Color[m_superSampling * m_width * m_superSampling * m_height]);
    
    int w = m_superSampling * m_width;
    int h = m_superSampling * m_height;
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
//            Ray r = m_camera.viewPointToRay((double)i/m_superSampling - w/2, (double)j/m_superSampling - h/2);
            Ray r = m_camera.viewPointToRay((double)i/m_superSampling, (double)j/m_superSampling);
            
            Color c;
            
            if(s.findIntersection(r, intersection)) {
                c = Color{255, 255, 255, 255};
            } else {
                c = Color{100, 200, 100, 255};
            }
            
            tempBuffer[j*w + i] = c;
        }
    }
    
    auto b = std::unique_ptr<Bitmap>(new Bitmap(m_width, m_height));
    for(int j = 0; j < m_height; ++j) {
        for(int i = 0; i < m_width; ++i) {
            Color c;
            for(int xs = 0; xs < m_superSampling; ++xs) {
                for(int ys = 0; ys < m_superSampling; ++ys) {
                    size_t index = (j+ys)*w + i+xs;
                    c += tempBuffer[index];
                }
            }
            
            c /= m_superSampling * m_superSampling;
            b->pixel(i, j) = c;
        }
    }
    
    return b;
}

void Renderer::prepareRender() {
    double focalLength = m_height / (2 * std::tan(0.5 * m_fovY));
    m_camera.setFocalLength(focalLength);
}
