//
//  Scene.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "Scene.h"

#include <algorithm>
#include <limits>

#include "BaseObject.h"
#include "Ray.h"
#include "Vector.h"

Scene::Scene() {
    
}

Scene::~Scene() {
    for(auto obj : m_objects) {
        delete obj;
    }
}

void Scene::addObject(BaseObject* obj) {
    if(std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end()) {
        m_objects.push_back(obj);
    }
}

BaseObject* Scene::findIntersection(const Ray& ray, Vector& intersection) const {
    const BaseObject* found = nullptr;
    
    for(const BaseObject* obj : m_objects) {
        Vector temp;
        if(!obj->intersects(ray, temp)) {
            continue;
        }
        
        if(found && (temp - ray.origin).lengthSqr() > (intersection - ray.origin).lengthSqr()) {
            continue;
        }
        
        intersection = temp;
        found = obj;
    }
    
    return const_cast<BaseObject*>(found);
}