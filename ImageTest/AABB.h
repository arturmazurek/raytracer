//
//  AABB.h
//  ImageTest
//
//  Created by Artur Mazurek on 12/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__AABB__
#define __ImageTest__AABB__

#include "Vector.h"

struct Ray;

struct AABB {
    Vector center;
    Vector extends;
    
    AABB(const Vector& center = {}, const Vector& extends = {});
    
    bool intersects(const Ray& r) const;
};

#endif /* defined(__ImageTest__AABB__) */
