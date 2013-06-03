//
//  Cylinder.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 03/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Cylinder.h"

#include <cassert>

#include "Ray.h"
#include "Util.h"

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
    Ray alteredRay = r;
    Vector alteredPosition = m_position;
    switch (m_axis) {
        case AxisAlignment::X_AXIS:
            alteredRay.origin.x = 0;
            alteredRay.direction.x = 0;
            alteredPosition.x = 0;
            break;
            
        case AxisAlignment::Y_AXIS:
            alteredRay.origin.y = 0;
            alteredRay.direction.y = 0;
            alteredPosition.y = 0;
            break;
            
        case AxisAlignment::Z_AXIS:
            alteredRay.origin.z = 0;
            alteredRay.direction.z = 0;
            alteredPosition.z = 0;
            break;
            
        default:
            assert(!"Shouldn't get here");
    }
    
    const FloatType A = alteredRay.origin.x - alteredPosition.x;
    const FloatType B = alteredRay.origin.y - alteredPosition.y;
    const FloatType C = alteredRay.origin.z - alteredPosition.z;
    
    const FloatType a = alteredRay.direction.lengthSqr();
    const FloatType b = 2*(A*alteredRay.direction.x + B*alteredRay.direction.y + C*alteredRay.direction.z);
    const FloatType c = A*A + B*B + C*C - m_radius*m_radius;
    
    using namespace std;
    const FloatType delta = b*b - 4*a*c;
    
    if(delta < 0) {
        return false;
    }
    
    const FloatType t1 = (-b - sqrt(delta)) / (2*a);
    const FloatType t2 = (-b + sqrt(delta)) / (2*a);
    
    if(t1 < 0 && t2 < 0) {
        return false;
    }
    
    return checkFactors(r, t1, t2, intersection, normal);
}

// t1 must be smaller than t2
bool Cylinder::checkFactors(const Ray& r, FloatType t1, FloatType t2, Vector& intersection, Vector& normal) const {
    intersection = {};
    normal = {};
    
    bool t1Valid = true;
    if(t1 >= 0) {
        intersection = r.origin + t1*r.direction;
        normal = intersection - m_position;
        
        switch (m_axis) {
            case AxisAlignment::X_AXIS:
                if(abs(intersection.x - m_position.x) > m_extends) {
                    t1Valid = false;
                }
                break;
                
            case AxisAlignment::Y_AXIS:
                if(abs(intersection.y - m_position.y) > m_extends) {
                    t1Valid = false;
                }
                break;
                
            case AxisAlignment::Z_AXIS:
                if(abs(intersection.z - m_position.z) > m_extends) {
                    t1Valid = false;
                }
                break;
                
            default:
                assert(!"Shouldn't get here");
        }
    } else {
        t1Valid = false;
    }
    
    bool t2Valid = true;
    Vector intersection2 = r.origin + t2*r.direction;
    Vector normal2 = intersection2 - m_position;
    if(t2 >= 0 && !t1Valid) {
        switch (m_axis) {
            case AxisAlignment::X_AXIS:
                if(abs(intersection2.x - m_position.x) > m_extends) {
                    t2Valid = false;
                }
                break;
                
            case AxisAlignment::Y_AXIS:
                if(abs(intersection2.y - m_position.y) > m_extends) {
                    t2Valid = false;
                }
                break;
                
            case AxisAlignment::Z_AXIS:
                if(abs(intersection2.z - m_position.z) > m_extends) {
                    t2Valid = false;
                }
                break;
                
            default:
                assert(!"Shouldn't get here");
        }
    } else {
        t2Valid = false;
    }
    
    if(!t1Valid && !t2Valid) {
        return false;
    } else if(t2Valid && !t1Valid) {
        normal = normal2;
        intersection = intersection2;
        normal *= -1;
    }
    
    switch (m_axis) {
        case AxisAlignment::X_AXIS:
            if(abs(intersection.x - m_position.x) > m_extends) {
                return false;
            }
            normal.x = 0;
            break;
            
        case AxisAlignment::Y_AXIS:
            if(abs(intersection.y - m_position.y) > m_extends) {
                return false;
            }
            normal.y = 0;
            break;
            
        case AxisAlignment::Z_AXIS:
            if(abs(intersection.z - m_position.z) > m_extends) {
                return false;
            }
            normal.z = 0;
            break;
            
        default:
            assert(!"Shouldn't get here");
    }
    normal.normalize();

    return true;
}
