//
//  BaseObject.cpp
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#include "BaseObject.h"

#include <sstream>

static const std::string DEFAULT_NAME = "object_";

int BaseObject::s_instanceCounter = 1;

BaseObject::BaseObject(const std::string& name) : m_name{name} {
    if(m_name.empty()) {
        std::stringstream ss;
        ss << DEFAULT_NAME << s_instanceCounter;
        ++s_instanceCounter;
        
        m_name = ss.str();
    }
}

void BaseObject::setName(const std::string& name) {
    m_name = name;
}
        
const std::string& BaseObject::name() const {
    return m_name;
}

bool BaseObject::intersects(const Ray& r, Vector& intersection, Vector& normal) const {
    if(m_AABB.intersects(r)) {
        return checkIntersection(r, intersection, normal);
    } else {
        return false;
    }
}

bool BaseObject::checkIntersection(const Ray& r, Vector& intersection, Vector& normal) const {
    return false;
}

AABB BaseObject::getAABB() const {
    return {};
}

void BaseObject::prepareObject() {
    m_AABB = getAABB();
}