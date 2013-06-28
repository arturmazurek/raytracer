//
//  Color.h
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Color__
#define __ImageTest__Color__

#include "Types.h"

struct Color {
    typedef FloatType ValueType;
    ValueType r, g, b, a;
    
    Color();
    Color(ValueType r, ValueType g, ValueType b, ValueType a);
    Color(const Color& other);
    Color& operator=(const Color& other);
    
    Color& operator+=(const Color& other);

    Color& operator*=(ValueType k);
    Color& operator*=(const Color& other);
    Color& operator/=(ValueType k);
    
    ValueType magnitudeSqr() const;
    
    static Color createFromIntegers(int r, int g, int b, int a);
};

Color operator*(const Color& c, Color::ValueType k);
Color operator*(Color::ValueType k, const Color& c);
Color operator*(const Color& a, const Color& b);

Color operator+(const Color& a, const Color& b);

bool operator==(const Color& a, const Color& b);
bool operator!=(const Color& a, const Color& b);

Color operator/(const Color& c, Color::ValueType k);

#endif /* defined(__ImageTest__Color__) */
