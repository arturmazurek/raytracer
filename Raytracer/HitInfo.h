//
//  HitInfo.h
//  Raytracer
//
//  Created by Artur Mazurek on 24.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __Raytracer__HitInfo__
#define __Raytracer__HitInfo__

#include "Vector.h"

class BaseObject;

struct HitInfo {
    Vector location;
    Vector normal;
    const BaseObject* obj;
};

#endif /* defined(__Raytracer__HitInfo__) */
