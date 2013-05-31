//
//  Renderer.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Renderer.h"

#include <cassert>
#include <memory>

#include "Bitmap.h"
#include "Ray.h"
#include "Scene.h"
#include "Vector.h"

Renderer::Renderer() : m_width{0}, m_height{0} {
    
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

std::unique_ptr<Bitmap> Renderer::renderScene(const Scene& s) const {
    Vector intersection;
    
    auto b = std::unique_ptr<Bitmap>(new Bitmap(m_width, m_height));
    
    int w = b->width();
    int h = b->height();
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Ray r;
            r.origin = Vector(i - w/2, j - h/2, 0);
            r.direction = Vector::unitZ();
            
            if(s.findIntersection(r, intersection)) {
                b->pixel(i, j) = Bitmap::PixelInfo(255, 255, 255, 255);
            } else {
                b->pixel(i, j) = Bitmap::PixelInfo(100, 200, 100, 255);
            }
        }
    }
    
    return b;
}
