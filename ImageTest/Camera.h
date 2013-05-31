//
//  Camera.h
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Camera__
#define __ImageTest__Camera__

#include "Ray.h"

class Camera {
public:
    Camera();
    virtual ~Camera();
    
    void setFocalLength(double focalLength);
    double focalLength() const;
    
    // x,y calculated from center
    Ray viewPointToRay(double x, double y) const;
    
private:
    double m_focalLength;
    
private:
    Camera(const Camera&);
    Camera& operator=(const Camera&);
};

#endif /* defined(__ImageTest__Camera__) */
