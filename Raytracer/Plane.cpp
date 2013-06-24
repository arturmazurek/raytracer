//
//  Plane.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 02.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Plane.h"

#include "HitInfo.h"
#include "Ray.h"
#include "Types.h"

Plane::Plane(const Vector& origin, const Vector& normal) : m_origin{origin}, m_normal{normal} {
    m_normal.normalize();
}

bool Plane::checkIntersection(const Ray& r, HitInfo& hit) const {
    FloatType nominator = dot(m_normal, {m_origin - r.origin});
    FloatType denominator = dot(m_normal, r.direction);
    
    // Todo: take numerical errors into account
    if(denominator == 0) {
        return false;
    }
    
    FloatType t = nominator / denominator;
    if(t < 0) {
        return false;
    } else {
        hit.location = r.origin + t*r.direction;
        hit.normal = m_normal;
        hit.obj = this;
        return true;
    }
}