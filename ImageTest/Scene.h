//
//  Scene.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Scene__
#define __ImageTest__Scene__

#include <vector>

class BaseObject;
struct Ray;
struct Vector;

class Scene {
public:
    Scene();
    ~Scene();
    
    void addObject(BaseObject* obj);
    
    BaseObject* findIntersection(const Ray& ray, Vector& intersection) const;
    
private:
    Scene(const Scene&);
    Scene& operator=(const Scene&);
    
private:
    std::vector<BaseObject*> m_objects;
};

#endif /* defined(__ImageTest__Scene__) */
