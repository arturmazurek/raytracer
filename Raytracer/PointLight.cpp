//
//  PointLight.cpp
//  Raytracer
//
//  Created by Artur Mazurek on 16.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "PointLight.h"

PointLight::PointLight(const Vector& position) : m_position{position} {
    
}

PointLight::~PointLight() {
    
}

BaseLight::Type PointLight::type() const {
    return BaseLight::Type::TYPE_POINT_LIGHT;
}

void PointLight::setPosition(const Vector& position) {
    m_position = position;
}

const Vector& PointLight::position() const {
    return m_position;
}