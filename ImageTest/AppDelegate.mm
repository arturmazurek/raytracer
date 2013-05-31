//
//  AppDelegate.m
//  ImageTest
//
//  Created by Artur Mazurek on 17/08/2012.
//  Copyright (c) 2012 Artur Mazurek. All rights reserved.
//

#import "AppDelegate.h"

#include <memory>

#include "Bitmap.h"
//#include "Ray.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
//#include "Vector.h"

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
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{Vector{-60, 0, 0}, 40}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{Vector{30, 30, 0}, 20}});
    s.addObject(std::unique_ptr<BaseObject>{new Sphere{Vector{0, 80, 0}, 20}});
    
    Renderer r;
    r.setDimensions(self.imageView.frame.size.width, self.imageView.frame.size.height);
    
    return r.renderScene(s);
}

@end
