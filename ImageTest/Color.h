//
//  Color.h
//  ImageTest
//
//  Created by Artur Mazurek on 31/05/2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef __ImageTest__Color__
#define __ImageTest__Color__

struct Color {
    typedef float ValueType;
    ValueType r, g, b, a;
    
    Color();
    Color(ValueType r, ValueType g, ValueType b, ValueType a);
    Color(const Color& other);
    Color& operator=(const Color& other);
    
    Color& operator+=(const Color& other);

    Color& operator/=(ValueType k);
    
    static Color createFromIntegers(int r, int g, int b, int a);
};

#endif /* defined(__ImageTest__Color__) */
