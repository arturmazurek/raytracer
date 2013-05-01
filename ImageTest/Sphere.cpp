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

#include "Ray.h"

Sphere::Sphere(const Vector& center, FloatType radius) {
    assert(radius != 0);
}

Sphere::~Sphere() {
    
}

bool Sphere::intersects(const Ray& r, Vector& intersection) const {
    const float A = r.origin.x - center.x;
    const float B = r.origin.y - center.y;
    const float C = r.origin.z - center.z;
    
    const float a = r.direction.lengthSqr();
    const float b = 2*A*r.direction.x + 2*B*r.direction.y + 2*C*r.direction.z;
    const float c = (A*A + B*B + C*C - radius*radius);
    
    using namespace std;
    const float delta = pow(b, 2) - 4*a*c;
    
    if(delta < 0) {
        return false;
    }
    
    const float t1 = (-b + sqrt(delta)) / (2*a);
    const float t2 = (-b - sqrt(delta)) / (2*a);
    
    if(t1 < 0 && t2 < 0) {
        return false;
    }
    
    if(t1 < 0) {
        return t2;
    }
    
    if(t2 < 0) {
        return t1;
    }
    
    return t2 < t1 ? t2 : t1;
}