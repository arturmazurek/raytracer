//
//  Types.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef ImageTest_Types_h
#define ImageTest_Types_h

typedef double FloatType;
//static_assert(sizeof(FloatType) > sizeof(double), "Oops not a real long double");

static inline long double max(long double a, long double b) {
    return a > b ? a : b;
}

#endif
