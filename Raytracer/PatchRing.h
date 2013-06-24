//
//  PatchRing.h
//  ImageTest
//
//  Created by Artur Mazurek on 11/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__PatchRing__
#define __ImageTest__PatchRing__

#include "BaseObject.h"
#include "Vector.h"

struct Ray;

class PatchRing : public BaseObject {
public:
    explicit PatchRing(const Vector& position = {}, const Vector& normal = {}, FloatType radiusA = 0, FloatType radiusB = 0);
    virtual ~PatchRing();
    
    void setPosition(const Vector& position);
    const Vector& position() const;
    
    void setNormal(const Vector& normal);
    const Vector& normal() const;
    
    void setRadiusA(FloatType radiusA);
    FloatType radiusA() const;
    
    void setRadiusB(FloatType radiusB);
    FloatType radiusB() const;
    
private:
    virtual bool checkIntersection(const Ray& r, HitInfo& hit) const override;
    virtual AABB getAABB() const override;
    
private:
    Vector m_position;
    Vector m_normal;
    FloatType m_radiusA;
    FloatType m_radiusB;
};

#endif /* defined(__ImageTest__PatchRing__) */
