//
//  BaseLight.h
//  ImageTest
//
//  Created by Artur Mazurek on 01.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__BaseLight__
#define __ImageTest__BaseLight__

#include "Color.h"
#include "Vector.h"

class BaseLight {
public:
    BaseLight();
    virtual ~BaseLight();
    
    void setPosition(const Vector& position);
    const Vector& position() const;
    
    void setColor(const Color& c);
    const Color& color() const;
    
    void setIntensity(double intensity);
    double intensity() const;
    
    double intensityAtPosition(const Vector& pos, const Vector& normal);
    
private:
    Vector m_position;
    Color m_color;
    double m_intensity;
};

#endif /* defined(__ImageTest__BaseLight__) */
