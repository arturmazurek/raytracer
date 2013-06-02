//
//  Types.h
//  ImageTest
//
//  Created by Artur Mazurek on 29/04/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef ImageTest_Types_h
#define ImageTest_Types_h

typedef long double FloatType;
static_assert(sizeof(FloatType) > sizeof(double), "Oops not a real long double");

#endif
