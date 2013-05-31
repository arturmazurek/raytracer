//
//  Bitmap.h
//  ImageTest
//
//  Created by Artur Mazurek on 30/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Bitmap__
#define __ImageTest__Bitmap__

#include <algorithm>
#include <memory>
#include <limits>

#include "Color.h"

class Bitmap {
public:
    typedef unsigned char byte;
    
    struct PixelInfo {
        static int bitsSize() {
            return sizeof(PixelInfo) * 8;
        }
        
        byte r, g, b, a;
        PixelInfo() : r{0}, g{0}, b{0}, a{0} {}
        PixelInfo(byte r, byte g, byte b, byte a) : r{r}, g{g}, b{b}, a{a} {}
        PixelInfo(const Color& c) {
            r = std::min(static_cast<Color::ValueType>(std::numeric_limits<byte>::max()), c.r);
            g = std::min(static_cast<Color::ValueType>(std::numeric_limits<byte>::max()), c.g);
            b = std::min(static_cast<Color::ValueType>(std::numeric_limits<byte>::max()), c.b);
            a = std::min(static_cast<Color::ValueType>(std::numeric_limits<byte>::max()), c.a);
        }
    };
    
    Bitmap(int width, int height);
    virtual ~Bitmap();
    
    int width() const;
    int height() const;
    const PixelInfo* data() const;
    
    const PixelInfo& pixel(int x, int y) const;
    PixelInfo& pixel(int x, int y);
    
private:
    int m_width;
    int m_height;
    std::unique_ptr<PixelInfo[]> m_data;
    
private:
    Bitmap(const Bitmap& other);
    Bitmap& operator=(const Bitmap& other);
};

#endif /* defined(__ImageTest__Bitmap__) */
