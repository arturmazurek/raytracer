//
//  Material.cpp
//  Raytracer
//
//  Created by Artur Mazurek on 24.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Material.h"

Material::Material() : m_color{1, 1, 1, 1}, m_emmitance{0, 0, 0, 1} {
    
}

Material::~Material() {
    
}

Color Material::shade(const HitInfo& hit) const {
    return getShading(hit);
}

const Color& Material::color() const {
    return m_color;
}

void Material::setColor(const Color& color) {
    m_color = color;
}

const Color& Material::emmitance() const {
    return m_emmitance;
}

void Material::setEmmitance(const Color& emmitance) {
    m_emmitance = emmitance;
}

Color Material::getShading(const HitInfo& hit) const {
    return m_color;
}