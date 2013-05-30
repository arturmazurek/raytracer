//
//  Renderer.h
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Renderer__
#define __ImageTest__Renderer__

#include <memory>

class Bitmap;
class Scene;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    std::unique_ptr<Bitmap> renderScene(Scene* s) const;
    
private:
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

#endif /* defined(__ImageTest__Renderer__) */