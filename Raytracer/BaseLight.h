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
    enum class Type {
        TYPE_POINT_LIGHT,
        TYPE_SPHERICAL_LIGHT,
        
        TYPE_ENUM_SIZE
    };
    
    BaseLight();
    virtual ~BaseLight();
    
    virtual Type type() const = 0;
    
    void setColor(const Color& c);
    const Color& color() const;
    
    void setIntensity(double intensity);
    double intensity() const;
    
private:
    Color m_color;
    double m_intensity;
};

#endif /* defined(__ImageTest__BaseLight__) */
