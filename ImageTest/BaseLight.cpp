//
//  BaseLight.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 01.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "BaseLight.h"

BaseLight::BaseLight() : m_position{}, m_color{1, 1, 1, 1}, m_intensity{1} {
    
}

BaseLight::~BaseLight() {
    
}

void BaseLight::setPosition(const Vector& position) {
    m_position = position;
}

const Vector& BaseLight::position() const {
    return m_position;
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
