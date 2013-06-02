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
#include "Ray.h"
#include "Vector.h"

Scene::Scene() {
    
}

Scene::~Scene() {

}

void Scene::addObject(std::unique_ptr<BaseObject> obj) {
    m_objects.push_back(std::move(obj));
}

void Scene::addLight(std::unique_ptr<BaseLight> light) {
    m_lights.push_back(std::move(light));
}

BaseObject* Scene::findIntersection(const Ray& ray, Vector& intersection, Vector& normal) const {
    const BaseObject* found = nullptr;
    
    for(const auto& obj : m_objects) {
        Vector temp;
        if(!obj->intersects(ray, temp, normal)) {
            continue;
        }
        
        if(found && (temp - ray.origin).lengthSqr() > (intersection - ray.origin).lengthSqr()) {
            continue;
        }
        
        intersection = temp;
        found = obj.get();
    }
    
    return const_cast<BaseObject*>(found);
}

Scene::LightsIterator Scene::lightsBegin() const {
    return m_lights.begin();
}

Scene::LightsIterator Scene::lightsEnd() const {
    return m_lights.end();
}