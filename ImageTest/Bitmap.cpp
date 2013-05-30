//
//  Bitmap.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 30/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Bitmap.h"

#include <cassert>

Bitmap::Bitmap(int width, int height) : m_width{width}, m_height{height}, m_data{new PixelInfo[width * height]} {
    
}

Bitmap::~Bitmap() {
    
}

int Bitmap::width() const {
    return m_width;
}

int Bitmap::height() const {
    return m_height;
}

const Bitmap::PixelInfo* Bitmap::data() const {
    return m_data.get();
}

const Bitmap::PixelInfo& Bitmap::pixel(int x, int y) const {
    int index = y*m_width + x;
    assert(index < m_width * m_height);
    
    return m_data[index];
}

Bitmap::PixelInfo& Bitmap::pixel(int x, int y) {
    return const_cast<PixelInfo&>(static_cast<const Bitmap&>(*this).pixel(x, y));
}