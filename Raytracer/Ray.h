//
//  Ray.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Ray__
#define __ImageTest__Ray__

#include "Vector.h"

struct Ray final {
    Vector origin;
    Vector direction;
    
    Ray() : Ray{{}, {}} {}
    Ray(const Vector& origin, const Vector& direction) : origin{origin}, direction{direction} {}
    Ray(const Ray& r) : origin{r.origin}, direction{r.direction} {}
};

#endif /* defined(__ImageTest__Ray__) */
