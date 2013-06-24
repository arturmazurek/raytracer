//
//  BaseObject.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__BaseObject__
#define __ImageTest__BaseObject__

#include "AABB.h"
#include "Material.h"
#include "Vector.h"

#include <memory>
#include <string>

class Material;
struct Ray;

class BaseObject {
public:
    explicit BaseObject(const std::string& name = {});
    virtual ~BaseObject() {}
    
    void setName(const std::string& name);
    const std::string& name() const;
    
    void prepareObject();
    
    bool intersects(const Ray& r, Vector& intersection, Vector& normal) const;
    
    Material* material();
    void setMaterial(std::unique_ptr<Material> material);
    
private:
    virtual bool checkIntersection(const Ray& r, Vector& intersection, Vector& normal) const;
    virtual AABB getAABB() const;
    
private:
    static int s_instanceCounter;
    
    std::string m_name;
    AABB m_AABB;
    
    std::unique_ptr<Material> m_material;
};

#endif /* defined(__ImageTest__BaseObject__) */
