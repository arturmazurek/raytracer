//
//  PointLight.h
//  Raytracer
//
//  Created by Artur Mazurek on 16.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __Raytracer__PointLight__
#define __Raytracer__PointLight__

#include "BaseLight.h"

#include "Vector.h"

class PointLight : public BaseLight {
public:
    PointLight(const Vector& position = {});
    virtual ~PointLight() override;
    
    BaseLight::Type type() const override;
    
    void setPosition(const Vector& position);
    const Vector& position() const;
    
private:
    Vector m_position;
};

#endif /* defined(__Raytracer__PointLight__) */
