//
//  Math.h
//  Wonderland
//
//  Created by Artur Mazurek on 15/03/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef Wonderland_Math_h
#define Wonderland_Math_h

#include "Types.h"

namespace Math {

    static const FloatType PI = 3.14159265359f;
    static const FloatType PI_INV = 0.31830988618f;
    static const FloatType PI2 = 9.86960440109f;
    static const FloatType DEG_2_RAD = PI / 180.f;
    static const FloatType RAD_2_DEG = 180.f / PI;
    static const FloatType EPSILON = 0.00001;
    
    static inline FloatType toDeg(float rad) {
        return rad * RAD_2_DEG;
    }
    
    static inline FloatType toRad(float deg) {
        return deg * DEG_2_RAD;
    }
    
};

#endif
