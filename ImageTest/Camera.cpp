//
//  Camera.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Camera.h"

#include <cassert>
#include <cmath>

#include "Math.h"

Camera::Camera() : m_focalLength{0} {
    
}

Camera::~Camera() {
    
}

void Camera::setFocalLength(double focalLength) {
    assert(focalLength >= 0);
    
    m_focalLength = focalLength;
}

double Camera::focalLength() const {
    return m_focalLength;
}

// x,y calculated from center
Ray Camera::viewPointToRay(double x, double y) const {
    double ry = y / m_focalLength;
    double rx = x / m_focalLength;
    
    Vector dir = {rx, ry, 1};
    dir.normalize();
    
    return {{x, y, 0}, dir};
}