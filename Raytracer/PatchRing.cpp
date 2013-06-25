//
//  PatchRing.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 11/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "PatchRing.h"

#include "HitInfo.h"
#include "Math.h"
#include "Ray.h"
#include "Util.h"

PatchRing::PatchRing(const Vector& position, const Vector& normal, FloatType radiusA, FloatType radiusB) : m_position{position}, m_normal{normal}, m_radiusA{radiusA}, m_radiusB{radiusB} {
    m_normal.normalize();
}

PatchRing::~PatchRing() {
    
}

void PatchRing::setPosition(const Vector& position) {
    m_position = position;
}

const Vector& PatchRing::position() const {
    return m_position;
}

void PatchRing::setNormal(const Vector& normal) {
    m_normal = normal;
    m_normal.normalize();
}

const Vector& PatchRing::normal() const {
    return m_normal;
}

void PatchRing::setRadiusA(FloatType radiusA) {
    m_radiusA = radiusA;
}

FloatType PatchRing::radiusA() const {
    return m_radiusA;
}

void PatchRing::setRadiusB(FloatType radiusB) {
    m_radiusB = radiusB;
}

FloatType PatchRing::radiusB() const {
    return m_radiusB;
}

bool PatchRing::checkIntersection(const Ray& r, HitInfo& hit) const {
    FloatType nominator = dot(m_normal, {m_position - r.origin});
    FloatType denominator = dot(m_normal, r.direction);
    
    // Todo: take numerical errors into account
    if(denominator == 0) {
        return false;
    }
    
    FloatType t = nominator / denominator;
    if(t < 0) {
        return false;
    }
    
    hit.location = r.origin + t*r.direction;
    
    FloatType diffSqr = (hit.location - m_position).lengthSqr();
    if(diffSqr > m_radiusB*m_radiusB) {
        return false;
    }
    
    if(diffSqr < m_radiusA*m_radiusA) {
        return false;
    }
    
    hit.normal = m_normal;
    if(dot(m_normal, r.direction) > 0) {
        hit.normal *= -1;
    }
    hit.obj = this;
    return true;
}

AABB PatchRing::getAABB() const {
    return {m_position, {m_radiusB, m_radiusB, m_radiusB}};
}