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

#include <cstdlib>

#include "BaseLight.h"
#include "BaseObject.h"
#include "HitInfo.h"
#include "Ray.h"
#include "Vector.h"

Scene::Scene() {
    
}

Scene::~Scene() {

}

void Scene::addObject(std::unique_ptr<BaseObject> obj) {
    m_objects.push_back(std::move(obj));
}

bool Scene::findIntersection(const Ray& ray, HitInfo& hit) const {
    bool found = false;
    
    HitInfo temp;
    for(const auto& obj : m_objects) {
        if(!obj->intersects(ray, temp)) {
            continue;
        }
        
        if(found && (temp.location - ray.origin).lengthSqr() > (hit.location - ray.origin).lengthSqr()) {
            continue;
        }
        
        hit = temp;
        found = true;
    }
    
    return found;
}

void Scene::prepare() {
    m_emitters.clear();
    
    for(auto& obj : m_objects) {
        obj->prepareObject();
        if(obj->material()->emmitance() != Color{}) {
            m_emitters.push_back(obj.get());
        }
    }
}

const std::vector<const BaseObject*>& Scene::allEmitters() const {
    return m_emitters;
}

