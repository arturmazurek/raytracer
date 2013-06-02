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

#include "Camera.h"
#include "Color.h"

class Bitmap;
class Scene;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    std::unique_ptr<Bitmap> renderScene(const Scene& s);

    void setDimensions(int width, int height);
    int width() const;
    int height() const;
    
    void setFovY(double fovYRadians);
    double fovY() const;
    
    void setSuperSampling(int amount);
    int getSuperSampling() const;
    
    void setFlipY(bool value);
    bool getFlipY() const;
    
private:
    int m_width;
    int m_height;
    double m_fovY;
    Camera m_camera;
    int m_superSampling;
    bool m_flipY;
    
private:
    void prepareRender();
    Color getDiffuse(const Scene& s, const Vector& pos, const Vector& normal);
    
private:
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

#endif /* defined(__ImageTest__Renderer__) */
