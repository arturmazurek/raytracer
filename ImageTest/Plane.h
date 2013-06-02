//
//  Plane.h
//  ImageTest
//
//  Created by Artur Mazurek on 02.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Plane__
#define __ImageTest__Plane__

#include "BaseObject.h"

#include "Vector.h"

struct Ray;

class Plane : public BaseObject {
public:
    explicit Plane(const Vector& origin = {}, const Vector& normal = {});
    
    virtual bool intersects(const Ray& r, Vector& intersection, Vector& normal) const override;
    
private:
    Vector m_origin;
    Vector m_normal;
};

#endif /* defined(__ImageTest__Plane__) */
