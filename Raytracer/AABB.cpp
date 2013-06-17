//
//  AABB.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 12/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "AABB.h"

#include <cmath>
#include <numeric>

#include "Ray.h"
#include "Vector.h"

AABB::AABB(const Vector& center, const Vector& extends) : center{center}, extends{extends} {
    
}

bool AABB::intersects(const Ray& r) const {    
    using namespace std;
    Vector moved = r.origin - center;
    
    if(abs(moved.x) > extends.x && moved.x * r.direction.x >= 0) {
        return false;
    }
    if(abs(moved.y) > extends.y && moved.y * r.direction.y > 0) {
        return false;
    }
    if(abs(moved.z) > extends.z && moved.z * r.direction.z > 0) {
        return false;
    }
    
    FloatType fAWdU[] = {
        abs(r.direction.x),
        abs(r.direction.y),
        abs(r.direction.z)
    };
    
	FloatType f;
    f = r.direction.y * moved.z - r.direction.z * moved.y; if(abs(f)>extends.y*fAWdU[2] + extends.z*fAWdU[1]) return false;
	f = r.direction.z * moved.x - r.direction.x * moved.z; if(abs(f)>extends.x*fAWdU[2] + extends.z*fAWdU[0]) return false;
	f = r.direction.x * moved.y - r.direction.y * moved.x; if(abs(f)>extends.x*fAWdU[1] + extends.y*fAWdU[0]) return false;
    
    return true;
}