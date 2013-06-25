//
//  Util.h
//  ImageTest
//
//  Created by Artur Mazurek on 03.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef ImageTest_Util_h
#define ImageTest_Util_h

#include <cstdlib>

#include "Math.h"
#include "Vector.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

static inline FloatType uniRand() {
    return (FloatType)(rand() % 1000000) / 1000000;
}

static inline Vector onSphereRand() {
    FloatType theta = 2 * Math::PI * uniRand();
    FloatType phi = acos(2*uniRand() - 1);
    
    return {sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)};
}

static inline Vector hemisphereRand(const Vector& normal) {
    Vector result = onSphereRand();
    FloatType k = dot(result, normal);
    if(k < 0) {
        result -= 2 * k * normal;
    }
    
    return result;
}

#endif
