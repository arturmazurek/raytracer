//
//  Scene.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Scene__
#define __ImageTest__Scene__

#include <memory>
#include <vector>

class BaseLight;
class BaseObject;
struct Ray;
struct Vector;

class Scene {
public:
    Scene();
    ~Scene();
    
    void addObject(std::unique_ptr<BaseObject> obj);
    void addLight(std::unique_ptr<BaseLight> light);
    
    BaseObject* findIntersection(const Ray& ray, Vector& intersection, Vector& normal) const;
    
    typedef std::vector<std::unique_ptr<BaseLight>>::const_iterator LightsIterator;
    LightsIterator lightsBegin() const;
    LightsIterator lightsEnd() const;
    
    void prepare();
    
private:
    Scene(const Scene&);
    Scene& operator=(const Scene&);
    
private:
    std::vector<std::unique_ptr<BaseObject>> m_objects;
    std::vector<std::unique_ptr<BaseLight>> m_lights;
};

#endif /* defined(__ImageTest__Scene__) */
