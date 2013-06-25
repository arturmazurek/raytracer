//
//  Cylinder.h
//  ImageTest
//
//  Created by Artur Mazurek on 03/06/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Cylinder__
#define __ImageTest__Cylinder__

#include "BaseObject.h"

#include <utility>

#include "Types.h"
#include "Vector.h"

class Cylinder : public BaseObject {
public:
    enum class AxisAlignment {
        X_AXIS,
        Y_AXIS,
        Z_AXIS,
    };
    
    enum class Orientation {
        INSIDE,
        OUTSIDE
    };
    
    Cylinder(const Vector& pos = {}, FloatType radius = 0, FloatType extends = 0, AxisAlignment axis = AxisAlignment::Y_AXIS);
    virtual ~Cylinder();
    
    void setPosition(const Vector& position);
    const Vector& position() const;
    
    void setRadius(FloatType radius);
    FloatType radius() const;
    
    void setExtends(FloatType extends);
    FloatType extends() const;
    
    void setAxisAlignment(AxisAlignment axis);
    AxisAlignment axisAlignment() const;
    
    void setOrientation(Orientation orientation);
    Orientation orientation() const;

private:
    virtual bool checkIntersection(const Ray& r, HitInfo& hit) const override;
    virtual AABB getAABB() const override;
    virtual void getRandomPoint(Vector& location, Vector& normal) const override;

    bool checkFactors(const Ray& r, FloatType t1, FloatType t2, HitInfo& hit) const;
    
private:
    Vector m_position;
    FloatType m_radius;
    FloatType m_extends;
    AxisAlignment m_axis;
    Orientation m_orientation;
};

#endif /* defined(__ImageTest__Cylinder__) */
