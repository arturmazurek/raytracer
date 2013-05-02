//
//  AppDelegate.m
//  ImageTest
//
//  Created by Artur Mazurek on 17/08/2012.
//  Copyright (c) 2012 Artur Mazurek. All rights reserved.
//

#import "AppDelegate.h"

#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Vector.h"

@implementation AppDelegate
@synthesize imageView = _imageView;

struct PixelInfo {
    static int bitsSize() {
        return sizeof(PixelInfo) * 8;
    }
    
    UInt8 r, g, b, a;
    PixelInfo() : r(0), g(0), b(0), a(0) {}
    PixelInfo(UInt8 r, UInt8 g, UInt8 b, UInt8 a) : r(r), g(g), b(b), a(a) {}
};

static const int N = 256;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    PixelInfo bytes[N*N];
    [self createImage:bytes size:N];
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CFDataRef imageData = CFDataCreate(kCFAllocatorDefault, (UInt8*)bytes, sizeof(bytes));
    CGDataProviderRef dataProvider = CGDataProviderCreateWithCFData(imageData);
    CGImageRef cgImage = CGImageCreate(N, N, 8, PixelInfo::bitsSize(), N * sizeof(PixelInfo), colorSpace, 0, dataProvider, NULL, TRUE, kCGRenderingIntentDefault);
    
    NSImage* img = [[NSImage alloc] initWithCGImage:cgImage size:NSZeroSize];
    
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);
    CGImageRelease(cgImage);
    CFRelease(imageData);
        
    [self.imageView setImage:img];
}

- (void)createImage:(PixelInfo*)pi size:(int)n {
    Scene s;
    s.addObject(new Sphere(Vector::zero(), 10));
    
    Vector intersection;
    
    for(int j = 0; j < N; ++j) {
        for(int i = 0; i < N; ++i) {
            Ray r;
            r.origin = Vector(i - N/2, j - N/2, 0);
            r.direction = Vector::unitZ();
            
            if(s.findIntersection(r, intersection)) {
                pi[j*N + i] = PixelInfo(255, 255, 255, 255);
            } else {
                pi[j*N + i] = PixelInfo(100, 200, 100, 255);
            }
        }
    }
}

@end
