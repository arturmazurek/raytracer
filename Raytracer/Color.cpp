//
//  Color.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Color.h"

Color::Color() : Color{0, 0, 0, 1} {
    
}

Color::Color(ValueType r, ValueType g, ValueType b, ValueType a) : r{r}, g{g}, b{b}, a{a} {
    
}

Color::Color(const Color& other) : r{other.r}, g{other.g}, b{other.b}, a{other.a} {
    
}

Color& Color::operator=(const Color& other) {
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
    
    return *this;
}

Color& Color::operator+=(const Color& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    
    return *this;
}

Color& Color::operator*=(ValueType k) {
    r *= k;
    g *= k;
    b *= k;
    
    return *this;
}

Color& Color::operator/=(ValueType k) { 
    r /= k;
    g /= k;
    b /= k;
    
    return *this;
}

Color::ValueType Color::magnitudeSqr() const {
    return r*r + g*g + b*b;
}

Color Color::createFromIntegers(int r, int g, int b, int a) {
    return {
        static_cast<ValueType>(r) / 255,
        static_cast<ValueType>(g) / 255,
        static_cast<ValueType>(b) / 255,
        static_cast<ValueType>(a) / 255 };
}

Color operator*(const Color& c, Color::ValueType k) {
    Color result{c};
    result *= k;
    return result;
}

Color operator*(Color::ValueType k, const Color& c) {
    return c * k;
}

Color operator/(const Color& c, Color::ValueType k) {
    Color result{c};
    result /= k;
    return result;
}
