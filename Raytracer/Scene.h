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
struct HitInfo;
struct Ray;
struct Vector;

class Scene {
public:
    Scene();
    ~Scene();
    
    void addObject(std::unique_ptr<BaseObject> obj);
    
    bool findIntersection(const Ray& ray, HitInfo& hit) const;

    void prepare();
    
private:
    Scene(const Scene&);
    Scene& operator=(const Scene&);
    
private:
    std::vector<std::unique_ptr<BaseObject>> m_objects;
};

#endif /* defined(__ImageTest__Scene__) */
