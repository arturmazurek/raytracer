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
#include "PatchRing.h"
#include "Plane.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Types.h"

@interface AppDelegate()

@property (nonatomic, assign) int bitmapWidth;
@property (nonatomic, assign) int bitmapHeight;

@end

@implementation AppDelegate
@synthesize imageView = _imageView;

static const FloatType RINGWORLD_RADIUS = 153000000000;
static const FloatType RINGWORLD_EXTENDS = 400000000;
static const FloatType RIM_WALL_HEIGHT = 1600000;
static const FloatType RIM_WALL_EXTENDS = 30;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.bitmapWidth = self.imageView.frame.size.width;
    self.bitmapHeight = self.imageView.frame.size.height;
    NSLog(@"Start");
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^() {
        [self render];
    });
}

- (void)showBitmap:(std::unique_ptr<Bitmap>)bitmap {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CFDataRef imageData = CFDataCreate(kCFAllocatorDefault, (UInt8*)bitmap->data(), sizeof(Bitmap::PixelInfo)*self.bitmapWidth*self.bitmapHeight);
    CGDataProviderRef dataProvider = CGDataProviderCreateWithCFData(imageData);
    CGImageRef cgImage = CGImageCreate(self.bitmapWidth, self.bitmapHeight, 8, Bitmap::PixelInfo::bitsSize(),
                                       self.bitmapWidth * sizeof(Bitmap::PixelInfo), colorSpace, 0, dataProvider, NULL, TRUE, kCGRenderingIntentDefault);
    
    NSImage* img = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
    
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);
    CGImageRelease(cgImage);
    CFRelease(imageData);
    
    [self.imageView setImage:img];
}

- (void)setupSpheres:(Scene&)s {
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{-1.5, -1, 10}, 1}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{1, 3, 10}, 1}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{-2, 4, 10}, 1}});
    
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{-1.5, -1, 100}, 1}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{1, 3, 100}, 1}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{-2, 4, 100}, 1}});
    
//    s.addObject(std::unique_ptr<BaseObject>{new Plane{{0, -2, 0}, {0, 1, 0}}});
    
    auto light = std::unique_ptr<BaseLight>(new BaseLight{});
    light->setPosition({10, 20, 0});
//    s.addLight(std::move(light));
}

- (void)setupRingworld:(Scene&)s {
    s.addObject(std::unique_ptr<BaseObject>{new Cylinder{{0, RINGWORLD_RADIUS - 2, 0}, RINGWORLD_RADIUS, RINGWORLD_EXTENDS, Cylinder::AxisAlignment::X_AXIS}});
    
    s.addObject(std::unique_ptr<BaseObject>{new PatchRing{{RINGWORLD_EXTENDS, RINGWORLD_RADIUS - 2, 0}, {-1, 0, 0}, RINGWORLD_RADIUS - RIM_WALL_HEIGHT, RINGWORLD_RADIUS}});
    s.addObject(std::unique_ptr<BaseObject>{new PatchRing{{-RINGWORLD_EXTENDS, RINGWORLD_RADIUS - 2, 0}, {1, 0, 0}, RINGWORLD_RADIUS - RIM_WALL_HEIGHT, RINGWORLD_RADIUS}});
    
    s.addObject(std::unique_ptr<BaseObject>{new Cylinder{{RINGWORLD_EXTENDS + RIM_WALL_EXTENDS, RINGWORLD_RADIUS - 2, 0}, RINGWORLD_RADIUS - RIM_WALL_HEIGHT, RIM_WALL_EXTENDS, Cylinder::AxisAlignment::X_AXIS}});
    s.addObject(std::unique_ptr<BaseObject>{new Cylinder{{-(RINGWORLD_EXTENDS + RIM_WALL_EXTENDS), RINGWORLD_RADIUS - 2, 0}, RINGWORLD_RADIUS - RIM_WALL_HEIGHT, RIM_WALL_EXTENDS, Cylinder::AxisAlignment::X_AXIS}});
    
    auto light = std::unique_ptr<BaseLight>(new BaseLight{});
    light->setPosition({0, RINGWORLD_RADIUS, 0});
    s.addLight(std::move(light));
}

- (void)setupPlanets:(Scene&)s {
    auto earth = std::unique_ptr<Sphere>{new Sphere{}};
    const FloatType earthRadius = 6378100;
    earth->setRadius(earthRadius);
    earth->setCenter({0, -(earthRadius + FloatType(2)), 0});
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
    
    auto light = std::unique_ptr<BaseLight>(new BaseLight{});
    light->setPosition({0, 2.5e17, 0});
    s.addLight(std::move(light));
}

- (void)render {
    Scene s;

//    [self setupPlanets:s];
    [self setupSpheres:s];
    [self setupRingworld:s];
    
    Renderer r;
    r.setDimensions(self.imageView.frame.size.width, self.imageView.frame.size.height);
    r.setSuperSampling(4);
    r.setFlipY(true);
    r.setExposure(1.5);
    r.setGamma(0.8);
    r.setBouncedRays(32);
    r.setMaxRayDepth(1);
    
    return r.renderScene(s, [self](const Bitmap& b, int progress) {
        if(progress == 100) {
            NSLog(@"End");
        }
        auto bitmap = b.copy().release();
        dispatch_sync(dispatch_get_main_queue(), ^() {
            [self showBitmap:std::unique_ptr<Bitmap>{bitmap}];
        });
    });
}

@end


