//
//  Vector.h
//  Wonderland
//
//  Created by Artur Mazurek on 06.02.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef Wonderland_Vector_h
#define Wonderland_Vector_h

#include <algorithm>
#include <cmath>
#include <limits>

#include "Types.h"

struct Vector final {
    FloatType x, y, z;
    
    Vector() : x(0), y(0), z(0) {}
    Vector(FloatType x, FloatType y, FloatType z) : x{x}, y{y}, z{z} {}
    Vector(const Vector& other) : x{other.x}, y{other.y}, z{other.z} {}
    Vector& operator=(const Vector& other) {
        x = other.x; y = other.y; z = other.z;
        return *this;
    }
    
    Vector& operator+=(const Vector& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    
    Vector& operator-=(const Vector& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    
    Vector& operator*=(FloatType k) {
        x *= k; y *= k; z *= k;
        return *this;
    }
    
    Vector& operator/=(FloatType k) {
        return (*this) *= (1.0 / k);
    }
    
    FloatType lengthSqr() const {
        return x*x + y*y + z*z;
    }
    
    FloatType length() const {
        return sqrt(lengthSqr());
    }
    
    Vector& normalize() {
        (*this) /= length();
        return *this;
    }
    
    static const Vector& zero() {
        static Vector result;
        return result;
    }
    
    static const Vector& unitX() {
        static Vector result{1, 0, 0};
        return result;
    }
    
    static const Vector& unitY() {
        static Vector result{0, 1, 0};
        return result;
    }
    
    static const Vector& unitZ() {
        static Vector result{0, 0, 1};
        return result;
    }
};

static inline bool operator==(const Vector& a, const Vector& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static inline bool operator!=(const Vector& a, const Vector& b) {
    return !(a == b);
}
    
static inline Vector operator-(const Vector& a) {
    Vector result;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    
    return result;
}

static inline Vector operator+(const Vector& a, const Vector& b) {
    Vector result{a};
    return result += b;
}

static inline Vector operator-(const Vector& a, const Vector& b) {
    Vector result{a};
    return result -= b;
}

static inline Vector operator*(const Vector& a, FloatType k) {
    Vector result{a};
    return result *= k;
}

static inline Vector operator*(FloatType k, const Vector& a) {
    return a * k;
}

static inline Vector operator/(const Vector& a, FloatType k) {
    return a * (1.0 / k);
}

static inline Vector normalized(const Vector& a) {
    Vector result{a};
    return result.normalize();
}

static inline FloatType dot(const Vector& a, const Vector& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline Vector cross(const Vector& a, const Vector& b) {
    Vector result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

static inline bool equal(const Vector& a, const Vector& b, FloatType epsilon = std::numeric_limits<FloatType>::epsilon()) {
    using namespace std;
    return abs(a.x-b.x) <= epsilon && abs(a.y-b.y) <= epsilon && abs(a.z-b.z) <= epsilon;
}
    
static inline Vector perpendicular(const Vector& a, const Vector& b) {
    return cross(a, b).normalize();
}

static inline Vector perpendicular(Vector to) {
    using namespace std;
    
    to.normalize();
    FloatType maxVal = max(max(to.x, to.y), to.z);
    if(maxVal == to.x) {
        return perpendicular(to, Vector::unitY());
    } else {
        return perpendicular(to, Vector::unitX());
    } 
}
    
#endif
    
