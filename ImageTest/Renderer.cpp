//
//  Renderer.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Renderer.h"

#include <cassert>

#include "Bitmap.h"
#include "Scene.h"
#include "Vector.h"

Renderer::Renderer() : m_width{0}, m_height{0} {
    
}

Renderer::~Renderer() {
    
}

void setDimensions(int width, int height) {
    
}

int width() const;
int height() const;

std::unique_ptr<Bitmap> Renderer::renderScene(Scene* s) const {
    Vector intersection;
    
    //    PixelInfo prevY;
    //    PixelInfo prevX;
    
    int w = b.width();
    int h = b.height();
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Ray r;
            r.origin = Vector(i - w/2, j - h/2, 0);
            r.direction = Vector::unitZ();
            
            if(s.findIntersection(r, intersection)) {
                b.pixel(i, j) = Bitmap::PixelInfo(255, 255, 255, 255);
            } else {
                b.pixel(i, j) = Bitmap::PixelInfo(100, 200, 100, 255);
            }
        }
    }
}
