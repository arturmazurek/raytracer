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
    explicit Sphere(const Vector& center = {}, FloatType radius = 0);
    virtual ~Sphere();
    
    void setCenter(const Vector& center);
    const Vector& center() const;
    
    void setRadius(FloatType radius);
    FloatType radius() const;
    
private:
    virtual bool checkIntersection(const Ray& r, Vector& intersection, Vector& normal) const override;
    
private:
    Vector m_center;
    FloatType m_radius;
};

#endif /* defined(__ImageTest__Sphere__) */
