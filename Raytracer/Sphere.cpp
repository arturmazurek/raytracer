//
//  Sphere.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Sphere.h"

#include <cassert>
#include <cmath>

#include "HitInfo.h"
#include "Ray.h"
#include "Util.h"

Sphere::Sphere(const Vector& center, FloatType radius) : m_center{center}, m_radius{radius} {

}

Sphere::~Sphere() {
    
}

void Sphere::setCenter(const Vector& center) {
    m_center = center;
}

const Vector& Sphere::center() const {
    return m_center;
}

void Sphere::setRadius(FloatType radius) {
    m_radius = radius;
}

FloatType Sphere::radius() const {
    return m_radius;
}

bool Sphere::checkIntersection(const Ray& r, HitInfo& hit) const {
    const FloatType A = r.origin.x - m_center.x;
    const FloatType B = r.origin.y - m_center.y;
    const FloatType C = r.origin.z - m_center.z;
    
    const FloatType a = r.direction.lengthSqr();
    const FloatType b = 2*(A*r.direction.x + B*r.direction.y + C*r.direction.z);
    const FloatType c = A*A + B*B + C*C - m_radius*m_radius;
    
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
    hit.location = r.origin + t*r.direction;
    
    hit.normal = hit.location - m_center;
    hit.normal.normalize();
    hit.obj = this;
    
    return t;
}

AABB Sphere::getAABB() const {
    return {m_center, {m_radius, m_radius, m_radius}};
}

void Sphere::getRandomPoint(Vector& location, Vector& normal) const {
    normal = onSphereRand();
    location = normal * m_radius;
    location += m_center;
}

FloatType Sphere::getSurfaceArea() const {
    return 4 * Math::PI * m_radius * m_radius;
}