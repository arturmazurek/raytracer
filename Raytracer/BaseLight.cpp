//
//  BaseLight.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 01.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "BaseLight.h"

#include <algorithm>

BaseLight::BaseLight() : m_color{1, 1, 1, 1}, m_intensity{1} {
    
}

BaseLight::~BaseLight() {
    
}

void BaseLight::setColor(const Color& c) {
    m_color = c;
}

const Color& BaseLight::color() const {
    return m_color;
}

void BaseLight::setIntensity(double intensity) {
    m_intensity = intensity;
}

double BaseLight::intensity() const {
    return m_intensity;
}
