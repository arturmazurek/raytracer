//
//  AppDelegate.m
//  ImageTest
//
//  Created by Artur Mazurek on 17/08/2012.
//  Copyright (c) 2012 Artur Mazurek. All rights reserved.
//

#import "AppDelegate.h"

#include "Bitmap.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"

@implementation AppDelegate
@synthesize imageView = _imageView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    int width = self.imageView.frame.size.width;
    int height = self.imageView.frame.size.height;
    
    Bitmap b{width, height};
    [self createImage:b];
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CFDataRef imageData = CFDataCreate(kCFAllocatorDefault, (UInt8*)b.data(), sizeof(Bitmap::PixelInfo)*width*height);
    CGDataProviderRef dataProvider = CGDataProviderCreateWithCFData(imageData);
    CGImageRef cgImage = CGImageCreate(width, height, 8, Bitmap::PixelInfo::bitsSize(), width * sizeof(Bitmap::PixelInfo), colorSpace, 0, dataProvider, NULL, TRUE, kCGRenderingIntentDefault);
    
    NSImage* img = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
    
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);
    CGImageRelease(cgImage);
    CFRelease(imageData);
        
    [self.imageView setImage:img];
}

- (void)createImage:(Bitmap&)b {
    Scene s;
    s.addObject(new Sphere(Vector::zero(), 40));
    
    Vector intersection;
    
//    PixelInfo prevY;
//    PixelInfo prevX;
    
    int w = b.width();
    int h = b.height();
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            Ray r;
            r.origin = Vector(i - w/2, j - h/2, 0);
            r.direction = Vector::unitZ();
            
            if(s.findIntersection(r, intersection)) {
                b.pixel(i, j) = Bitmap::PixelInfo(255, 255, 255, 255);
            } else {
                b.pixel(i, j) = Bitmap::PixelInfo(100, 200, 100, 255);
            }
        }
    }
}

@end
