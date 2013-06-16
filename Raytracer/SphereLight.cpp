//
//  SphereLight.cpp
//  Raytracer
//
//  Created by Artur Mazurek on 16.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "SphereLight.h"

SphereLight::SphereLight(const Vector& center, FloatType radius) : m_center{center}, m_radius{radius} {
    
}

SphereLight::~SphereLight() {
    
}

BaseLight::Type SphereLight::type() const {
    return BaseLight::Type::TYPE_SPHERE_LIGHT;
}

void SphereLight::setCenter(const Vector& center) {
    m_center = center;
}

const Vector& SphereLight::center() const {
    return m_center;
}

void SphereLight::setRadius(FloatType radius) {
    m_radius = radius;
}

FloatType SphereLight::radius() const {
    return m_radius;
}
