//
//  Cylinder.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 03/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Cylinder.h"

#include "Ray.h"

Cylinder::Cylinder(const Vector& pos, FloatType radius, FloatType extends, AxisAlignment axis) : m_position{pos}, m_radius{radius}, m_extends{extends}, m_axis{axis} {
    
}

Cylinder::~Cylinder() {
    
}

void Cylinder::setPosition(const Vector& position) {
    m_position = position;
}

const Vector& Cylinder::position() const {
    return m_position;
}

void Cylinder::setRadius(FloatType radius) {
    m_radius = radius;
}

FloatType Cylinder::radius() const {
    return m_radius;
}

void Cylinder::setExtends(FloatType extends) {
    m_extends = extends;
}

FloatType Cylinder::extends() const {
    return m_extends;
}

void Cylinder::setAxisAlignment(AxisAlignment axis) {
    m_axis = axis;
}

Cylinder::AxisAlignment Cylinder::axisAlignment() const {
    return m_axis;
}

bool Cylinder::intersects(const Ray& r, Vector& intersection, Vector& normal) const {
//    const FloatType A = r.origin.x - m_position.x;
    const FloatType B = r.origin.y - m_position.y;
    const FloatType C = r.origin.z - m_position.z;
    
    const FloatType a = r.direction.y*r.direction.y + r.direction.z*r.direction.z;
    const FloatType b = 2*(/*A*r.direction.x*/ + B*r.direction.y + C*r.direction.z);
    const FloatType c = /*A*A*/ + B*B + C*C - m_radius*m_radius;
    
    using namespace std;
    const FloatType delta = b*b - 4*a*c;
    
    if(delta < 0) {
        return false;
    }
    
    const FloatType t1 = (-b + sqrt(delta)) / (2*a);
    const FloatType t2 = (-b - sqrt(delta)) / (2*a);
    
    if(t1 < 0 && t2 < 0) {
        return false;
    }
    
    FloatType t = t2 < t1 ? t2 : t1;
    intersection = r.origin + t*r.direction;
    
    normal = intersection - m_position;
    normal.x = 0;
    normal.normalize();
    
    return t;
}