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

- (std::unique_ptr<Bitmap>)createImage {
    Scene s;
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{0, 0, 50}, 40}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{50, 50, 50}, 20}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{{0, 80, 50}, 20}});
    
    auto light = std::unique_ptr<BaseLight>(new BaseLight{});
    light->setPosition({50, 50, 0});
    s.addLight(std::move(light));
    
    Renderer r;
    r.setDimensions(self.imageView.frame.size.width, self.imageView.frame.size.height);
    r.setSuperSampling(2);
    r.setFlipY(true);
    
    return r.renderScene(s);
}

@end
