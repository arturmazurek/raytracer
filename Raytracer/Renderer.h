//
//  Renderer.h
//  ImageTest
//
//  Created by Artur Mazurek on 21/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Renderer__
#define __ImageTest__Renderer__

#include <deque>
#include <functional>
#include <list>
#include <memory>
#include <thread>
#include <vector>

#include "Camera.h"
#include "Color.h"

class Bitmap;
class PointLight;
class Ray;
class Scene;
class SphereLight;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // function is a callback returning the current state of bitmap, and int [0-100] with the percentage of the process
    void renderScene(Scene& s, std::function<void(const Bitmap&, int)> callback);

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
    
    void setBouncedRays(int count);
    int bouncedRays() const;
    
    void setMaxRayDepth(int depth);
    int maxRayDepth() const;
    
private:
    struct Block {
        int x, y;
        int w, h;
        
        int totalW, totalH;
        
        int iterations;
    };
    
private:
    static const int BLOCK_DEFAULT_W;
    static const int BLOCK_DEFAULT_H;
    
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
    int m_bouncedRays;
    int m_maxRayDepth;
    
    int m_pathTracingIteration;
    
private:
    void prepareRender(Scene& s);
    
    void scaleDown(Color* fromBuffer, Bitmap& toBitmap, const Block& block, bool overwrite = false) const;
    std::deque<Block> prepareBlocks(int blockWidth = BLOCK_DEFAULT_W, int blockHeight = BLOCK_DEFAULT_H) const;
    
    void pathTraceScene(Scene& s, std::function<void(const Bitmap&, int)> callback, int iterations);
    void pathTracing(const Scene& s, Color* result, const Block& block, int pixelIters, int total);
    Color tracePath(const Scene& s, const Ray& r, int depthLeft) const;
    
    void processImage(Color* bitmap, const Block& block, std::function<void(Color&)> filter) const;
    void correctGamma(Color& c) const;
    void correctExposure(Color& c) const;
    
    void processParallel(std::function<void()> worker) const;
    
private:
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

#endif /* defined(__ImageTest__Renderer__) */
