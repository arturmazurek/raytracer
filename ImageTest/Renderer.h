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
class Ray;
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
    
    void setRayBias(double bias);
    double rayBias() const;
    
    void setExposure(double exposure);
    double exposure() const;
    
    void setGamma(double gamma);
    double gamma() const;
    
    void setOcclusionRays(int count);
    int occlusionRays() const;
    
    void setOcclusionRaysBounces(int count);
    int occlusionRaysBounces() const;
    
private:
    int m_width;
    int m_height;
    double m_fovY;
    Camera m_camera;
    int m_superSampling;
    bool m_flipY;
    double m_rayBias;
    double m_exposure;
    double m_gamma;
    double m_highestIntensity;
    int m_occlusionRays;
    int m_occlusionRaysBounces;
    
private:
    void prepareRender();
    Color getDiffuse(const Scene& s, const Vector& pos, const Vector& normal);
    void raycast(const Scene& s, Color* result, int width, int height);
    Color processRay(const Scene& s, const Ray& r);
    void processExposure(Color* buffer, int w, int h) const;
    Color bouncedDiffuseAtPosition(const Scene& s, const Vector& pos, const Vector& normal) const;
    std::unique_ptr<Bitmap> scaleDown(std::unique_ptr<Color[]> rawBuffer) const;
    void correctGamma(Bitmap& b, int width, int height) const;
    
private:
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

#endif /* defined(__ImageTest__Renderer__) */
