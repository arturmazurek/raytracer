//
//  Bitmap.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 30/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Bitmap.h"

#include <cassert>
#include <cstring>

Bitmap::Bitmap(int width, int height) : m_width{width}, m_height{height}, m_data{new PixelInfo[width * height]} {
    
}

Bitmap::~Bitmap() {
    
}

std::unique_ptr<Bitmap> Bitmap::copy() const {
    auto result = std::unique_ptr<Bitmap>{new Bitmap{m_width, m_height}};
    copyTo(*result);
    
    return std::move(result);
}

void Bitmap::copyTo(Bitmap& other) const {
    assert(m_width == other.m_width);
    assert(m_height == other.m_height);
    
    memcpy(other.m_data.get(), m_data.get(), m_width * m_height * sizeof(PixelInfo));
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
    assert(x < m_width);
    assert(y < m_height);
    int index = y*m_width + x;
    return m_data[index];
}

Bitmap::PixelInfo& Bitmap::pixel(int x, int y) {
    return const_cast<PixelInfo&>(static_cast<const Bitmap&>(*this).pixel(x, y));
}