//
//  SphereLight.h
//  Raytracer
//
//  Created by Artur Mazurek on 16.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __Raytracer__SphereLight__
#define __Raytracer__SphereLight__

#include "BaseLight.h"

#include "Types.h"
#include "Vector.h"

class SphereLight : public BaseLight {
public:
    SphereLight(const Vector& center = {}, FloatType radius = 0);
    virtual ~SphereLight() override;
    
    BaseLight::Type type() const override;
    
    void setCenter(const Vector& center);
    const Vector& center() const;
    
    void setRadius(FloatType radius);
    FloatType radius() const;
    
private:
    Vector m_center;
    FloatType m_radius;
};

#endif /* defined(__Raytracer__SphereLight__) */
