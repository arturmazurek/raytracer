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
    unsigned depth;
    
    Ray() : Ray{{}, {}, 0} {}
    Ray(const Vector& origin, const Vector& direction, unsigned depth = 0) : origin{origin}, direction{direction}, depth{depth} {}
    Ray(const Ray& r) : origin{r.origin}, direction{r.direction}, depth{r.depth} {}
};

#endif /* defined(__ImageTest__Ray__) */
