//
//  Sphere.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Sphere__
#define __ImageTest__Sphere__

#include "BaseObject.h"

#include "Types.h"
#include "Vector.h"

class Sphere : public BaseObject {
public:
    Sphere(const Vector& center, FloatType radius);
    virtual ~Sphere();
    
    virtual bool intersects(const Ray& r, Vector& intersection) const override;
    
private:
    Vector center;
    FloatType radius;
};

#endif /* defined(__ImageTest__Sphere__) */
