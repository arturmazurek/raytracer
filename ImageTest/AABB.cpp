//
//  AABB.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 12/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "AABB.h"

#include <numeric>

#include "Ray.h"

AABB::AABB(const Vector& center, const Vector& extends) {
    
}

bool AABB::intersects(const Ray& r) const {
    return true;
}