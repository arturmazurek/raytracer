//
//  AppDelegate.m
//  ImageTest
//
//  Created by Artur Mazurek on 17/08/2012.
//  Copyright (c) 2012 Artur Mazurek. All rights reserved.
//

#import "AppDelegate.h"

#include <memory>

#include "BaseLight.h"
#include "Bitmap.h"
#include "Cylinder.h"
#include "Math.h"
#include "Plane.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"

@implementation AppDelegate
@synthesize imageView = _imageView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    int width = self.imageView.frame.size.width;
    int height = self.imageView.frame.size.height;
    
    auto bitmap = [self createImage];
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CFDataRef imageData = CFDataCreate(kCFAllocatorDefault, (UInt8*)bitmap->data(), sizeof(Bitmap::PixelInfo)*width*height);
    CGDataProviderRef dataProvider = CGDataProviderCreateWithCFData(imageData);
    CGImageRef cgImage = CGImageCreate(width, height, 8, Bitmap::PixelInfo::bitsSize(), width * sizeof(Bitmap::PixelInfo), colorSpace, 0, dataProvider, NULL, TRUE, kCGRenderingIntentDefault);
    
    NSImage* img = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
    
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);
    CGImageRelease(cgImage);
    CFRelease(imageData);
        
    [self.imageView setImage:img];
}

- (void)setupSpheres:(Scene&)s {
//    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{0, 0, 50}, 40}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{60, 50, 50}, 20}});
//    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{-60, 80, 50}, 20}});
    s.addObject(std::unique_ptr<BaseObject>{new Cylinder{{0, 100, 45}, 10, 10, Cylinder::AxisAlignment::Z_AXIS}});
    
    s.addObject(std::unique_ptr<BaseObject>{new Plane{{0, -120, 0}, {0, 1, 0}}});
}

- (void)setupPlanets:(Scene&)s {
    auto earth = std::unique_ptr<Sphere>{new Sphere{}};
    const FloatType earthRadius = 6378100;
    earth->setRadius(earthRadius);
    earth->setCenter({0, -(earthRadius + FloatType(200)), 0});
    earth->setName("earth");
    s.addObject(std::move(earth));
    
    auto moon = std::unique_ptr<Sphere>{new Sphere{}};
    const FloatType moonRadius = 1735970;
    moon->setRadius(moonRadius);
    const FloatType moonDistance = 36257000;
    const FloatType angle = Math::PI * FloatType(0.1);
    moon->setCenter({0, moonDistance * sin(angle), moonDistance * cos(angle)});
    moon->setName("moon");
    s.addObject(std::move(moon));
}

- (std::unique_ptr<Bitmap>)createImage {
    Scene s;

//    [self setupPlanets:s];
    [self setupSpheres:s];
    
    auto light = std::unique_ptr<BaseLight>(new BaseLight{});
//    light->setPosition({0, 200, 0});
    light->setPosition({0, 2.5e17, 0});
    
    s.addLight(std::move(light));
    
    Renderer r;
    r.setDimensions(self.imageView.frame.size.width, self.imageView.frame.size.height);
    r.setSuperSampling(1);
    r.setFlipY(true);
    
    return r.renderScene(s);
}

@end
