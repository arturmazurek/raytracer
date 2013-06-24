//
//  Material.h
//  Raytracer
//
//  Created by Artur Mazurek on 24.06.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __Raytracer__Material__
#define __Raytracer__Material__

#include "Color.h"
#include "Vector.h"

struct HitInfo;

class Material {
public:
    Material();
    virtual ~Material();
    
    Color shade(const HitInfo& hit) const;
    
    const Color& color() const;
    void setColor(const Color& color);
    
    const Color& emmitance() const;
    void setEmmitance(const Color& emmitance);
    
protected:
    virtual Color getShading(const HitInfo& hit) const;
    
private:
    Material(const Material&);
    Material& operator=(const Material&);
    
private:
    Color m_color;
    Color m_emmitance;
};

#endif /* defined(__Raytracer__Material__) */
