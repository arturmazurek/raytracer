//
//  BaseObject.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__BaseObject__
#define __ImageTest__BaseObject__

struct Ray;
struct Vector;

class BaseObject {
public:
    virtual ~BaseObject() {}
    
    virtual bool intersects(const Ray& r, Vector& intersection) const = 0;
};

#endif /* defined(__ImageTest__BaseObject__) */
