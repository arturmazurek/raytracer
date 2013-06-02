//
//  BaseObject.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__BaseObject__
#define __ImageTest__BaseObject__

#include "Vector.h"

#include <string>

struct Ray;

class BaseObject {
public:
    explicit BaseObject(const std::string& name = {});
    virtual ~BaseObject() {}
    
    void setName(const std::string& name);
    const std::string& name() const;
    
    virtual bool intersects(const Ray& r, Vector& intersection) const = 0;
    virtual Vector normalAtPoint(const Vector& point) const = 0;
    
private:
    static int s_instanceCounter;
    
    std::string m_name;
};

#endif /* defined(__ImageTest__BaseObject__) */
