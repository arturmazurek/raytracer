//
//  Color.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Color.h"

Color::Color() : Color{0, 0, 0, 0} {
    
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
    a += other.a;
    
    return *this;
}

Color& Color::operator/=(int k) {
    r = (ValueType)((double)r / k + 0.5);
    g = (ValueType)((double)g / k + 0.5);
    b = (ValueType)((double)b / k + 0.5);
    a = (ValueType)((double)a / k + 0.5);
    
    return *this;
}