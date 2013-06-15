//
//  Matrix.h
//  Wonderland
//
//  Created by Artur Mazurek on 06.02.2013.
//  Copyright (c) 2013 Artur Mazurek. All rights reserved.
//

#ifndef Wonderland_Matrix_h
#define Wonderland_Matrix_h

#include <cstring>
#include <cmath>

#include <algorithm>

#include "Types.h"
#include "Vector.h"

// m00 m01 m02 m03
// m10 m11 m12 m13
// m20 m21 m22 m23
// m30 m31 m32 m33
struct Matrix {
    static const int N = 4;
    
    // column-major ordering
    FloatType m[N*N];
    
    Matrix() { memset(m, 0, sizeof(m)); }
    Matrix(const Matrix& other) { std::memcpy(m, other.m, sizeof(m)); }
    Matrix(FloatType *elements) { std::memcpy(m, elements, sizeof(m)); }
    Matrix& operator=(const Matrix& other) { std::memcpy(m, other.m, sizeof(m)); return *this; }
    
    static int index(int i, int j) {
        return 4*j + i;
    }
    
    void debugPrint() const {
//        LOG("%Lf %Lf %Lf %Lf", m[0], m[4], m[8], m[12]);
//        LOG("%Lf %Lf %Lf %Lf", m[1], m[5], m[9], m[13]);
//        LOG("%Lf %Lf %Lf %Lf", m[2], m[6], m[10], m[14]);
//        LOG("%Lf %Lf %Lf %Lf", m[3], m[7], m[11], m[15]);
        LOG("Matrix debug print unimplemented");
    }
    
    Matrix& operator*=(FloatType k) {
        for(int i = 0; i < 16; ++i) {
            m[i] *= k;
        }
        return *this;
    }
    
    Matrix& operator/=(FloatType k) {
        return (*this) *= (1.0f / k);
    }
    
    Vector getColumn(int j) {
        return Vector(m[index(0, j)], m[index(1, j)], m[index(2, j)]);
    }
    
    void setColumn(int j, const Vector& v) {
        m[index(0, j)] = v.x;
        m[index(1, j)] = v.y;
        m[index(2, j)] = v.z;
    }
    
    Matrix& setTranslation(const Vector& v) {
        m[index(0, 3)] = v.x;
        m[index(1, 3)] = v.y;
        m[index(2, 3)] = v.z;
        return *this;
    }
    
    Matrix& translate(const Vector& v) {
        m[index(0, 3)] += v.x;
        m[index(1, 3)] += v.y;
        m[index(2, 3)] += v.z;
        return *this;
    }
    
    Matrix& setScale(FloatType s) {
        return setScale(s, s, s);
    }
    
    Matrix& setScale(FloatType sx, FloatType sy, FloatType sz) {
        FloatType scales[] = {sx, sy, sz};
        
        for(int i = 0; i < 3; ++i) {
            Vector temp = getColumn(i);
            temp.normalize();
            temp *= scales[i];
            setColumn(i, temp);
        }
        
        return *this;
    }
    
    Matrix& scale(FloatType s) {
        return scale(s, s, s);
    }
    
    Matrix& scale(FloatType sx, FloatType sy, FloatType sz) {
        m[index(0, 0)] *= sx; m[index(1, 0)] *= sx; m[index(2, 0)] *= sx;
        m[index(0, 1)] *= sy; m[index(1, 1)] *= sy; m[index(2, 1)] *= sy;
        m[index(0, 2)] *= sz; m[index(1, 2)] *= sz; m[index(2, 2)] *= sz;
        
        return *this;
    }
    
public:
    
    Matrix& invert() {
        Matrix inverse = createIdentity();
        int i, j, k, swap;
        FloatType t;
        
        for (i = 0; i < 4; ++i) {
            /*
             ** Look for largest element in column
             */
            swap = i;
            for (j = i + 1; j < 4; ++j) {
                if (std::abs(m[index(i, j)]) > std::abs(m[index(i, i)])) {
                    swap = j;
                }
            }
            
            if (swap != i) {
                /*
                 ** Swap rows.
                 */
                for (k = 0; k < 4; ++k) {
                    t = m[index(k, i)];
                    m[index(k, i)] = m[index(k, swap)];
                    m[index(k, swap)] = t;
                    
                    t = inverse.m[index(k, i)];
                    inverse.m[index(k, i)] = inverse.m[index(k, swap)];
                    inverse.m[index(k, swap)] = t;
                }
            }
            
            if (m[index(i, i)] == 0) {
                LOG("Oops, matrix with zero determinant given, not inverting.");
                return *this;
            }
            
            t = m[index(i, i)];
            for (k = 0; k < 4; ++k) {
                m[index(k, i)] /= t;
                inverse.m[index(k, i)] /= t;
            }
            for (j = 0; j < 4; ++j) {
                if (j == i) {
                    continue;
                }
                t = m[index(i, j)];
                for (k = 0; k < 4; ++k) {
                    m[index(k, j)] -= m[index(k, i)]*t;
                    inverse.m[index(k, j)] -= inverse.m[index(k, i)]*t;
                }
            }
        }
        
        std::memcpy(m, inverse.m, sizeof(m));
        return *this;
    }
    
    Matrix inverted() const {
        Matrix result(*this);
        result.invert();
        return result;
    }
    
    static Matrix createIdentity() {
        Matrix result;
        result.m[0] = 1;
        result.m[index(1, 1)] = 1;
        result.m[index(2, 2)] = 1;
        result.m[index(3, 3)] = 1;
        return result;
    }
    
    static Matrix createScale(FloatType s) {
        return createScale(s, s, s);
    }
    
    static Matrix createScale(FloatType sx, FloatType sy, FloatType sz) {
        Matrix result;
        result.m[index(0, 0)] = sx;
        result.m[index(1, 1)] = sy;
        result.m[index(2, 2)] = sz;
        result.m[index(3, 3)] = 1;
        
        return result;
    }
    
    static Matrix createTranslation(const Vector& v) {
        Matrix result = createIdentity();
        result.m[index(0, 3)] = v.x;
        result.m[index(1, 3)] = v.y;
        result.m[index(2, 3)] = v.z;
        
        return result;
    }
    
    static Matrix createPerspective(FloatType fovy, FloatType aspect, FloatType near, FloatType far) {
        Matrix result;
        
        FloatType f = 1.f / std::tan(fovy * 0.5f);
        result.m[index(0, 0)] = f / aspect;
        result.m[index(1, 1)] = f;
        result.m[index(2, 2)] = (far + near) / (near - far);
        result.m[index(3, 2)] = -1;
        result.m[index(2, 3)] = (2 * far * near) / (near - far);
        
        return result;
    }
    
    static Matrix createOrtho(FloatType width, FloatType height, FloatType near, FloatType far) {
        Matrix result;
        
        result.m[index(0, 0)] = 2.0f / width;
        result.m[index(1, 1)] = 2.0f / height;
        result.m[index(2, 2)] = -2.0f / (far - near);
        result.m[index(3, 3)] = 1.0f;
        
        result.m[index(2, 3)] = -(far + near) / (far - near);
        
        return result;
    }
    
    // yaw is rotation around axis Y
    // pitch is rotation around axis Z
    // roll is rotation around axis X
    static Matrix createRotation(FloatType rotX, FloatType rotY, FloatType rotZ) {
        using namespace std;
        
        Matrix result;
        
        const FloatType phi = rotX;
        const FloatType theta = rotY;
        const FloatType psi = rotZ;
        
        // going in columns
        result.m[index(0,0)] = cos(theta) * cos(psi);
        result.m[index(1,0)] = cos(theta)*sin(psi);
        result.m[index(2,0)] = -sin(theta);
        
        result.m[index(0,1)] = -cos(phi)*sin(psi) + sin(phi)*sin(theta)*cos(psi);
        result.m[index(1,1)] = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi);
        result.m[index(2,1)] = sin(phi)*cos(theta);
        
        result.m[index(0,2)] = sin(phi)*sin(psi) + cos(phi)*sin(theta)*cos(psi);
        result.m[index(1,2)] = -sin(phi)*cos(psi) + cos(phi)*sin(theta)*sin(psi);
        result.m[index(2,2)] = cos(phi)*cos(theta);
        
        result.m[index(3,3)] = 1.0f;
        
        return result;
    }
    
    // axis must be normalised
    static Matrix createRotation(const Vector& axis, FloatType angle) {
        Matrix result;
        
        const FloatType s = std::sin(angle);
        const FloatType c = std::cos(angle);
        
        // going in columns
        result.m[index(0, 0)] = c + axis.x*axis.x*(1.0f - c);
        result.m[index(1, 0)] = axis.y*axis.x*(1.0f - c) + axis.z*s;
        result.m[index(2, 0)] = axis.z*axis.x*(1.0f - c) - axis.y*s;
        
        result.m[index(0, 1)] = axis.x*axis.y*(1.0f - c) - axis.z*s;
        result.m[index(1, 1)] = c + axis.y*axis.y*(1.0f - c);
        result.m[index(2, 1)] = axis.z*axis.y*(1.0f - c) + axis.x*s;
        
        result.m[index(0, 2)] = axis.x*axis.z*(1.0f - c) + axis.y*s;
        result.m[index(1, 2)] = axis.y*axis.z*(1.0f - c) - axis.x*s;
        result.m[index(2, 2)] = c + axis.z*axis.z*(1.0f - c);
        
        return result;
    }
    
    
    };
    
    static Matrix operator*(const Matrix& mA, const Matrix& mB) {
        Matrix result;
        
        const FloatType* a = mA.m;
        const FloatType* b = mB.m;
        
        result.m[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
        result.m[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3];
        result.m[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
        result.m[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];
        
        result.m[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
        result.m[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
        result.m[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
        result.m[7] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];
        
        result.m[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
        result.m[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
        result.m[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
        result.m[11] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];
        
        result.m[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
        result.m[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
        result.m[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
        result.m[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
        
        return result;
    }
    
    // Implies w == 1
    static Vector multiplyPoint(const Matrix& a, const Vector& v) {
        Vector result;
        
        FloatType wInv = 1.0f / a.m[15];
        
        result.x = (a.m[0]*v.x + a.m[4]*v.y + a.m[8]*v.z + a.m[12]) * wInv;
        result.y = (a.m[1]*v.x + a.m[5]*v.y + a.m[9]*v.z + a.m[13]) * wInv;
        result.z = (a.m[2]*v.x + a.m[6]*v.y + a.m[10]*v.z + a.m[14]) * wInv;
        
        return result;
    }
    
    // Implies w == 0;
    static Vector multiplyDirection(const Matrix& a, const Vector& v) {
        Vector result;
        
        result.x = a.m[0]*v.x + a.m[4]*v.y + a.m[8]*v.z;
        result.y = a.m[1]*v.x + a.m[5]*v.y + a.m[9]*v.z;
        result.z = a.m[2]*v.x + a.m[6]*v.y + a.m[10]*v.z;
        
        return result;
    }
    
    // assumes we're multiplying a point
    static Vector operator*(const Matrix& m, const Vector& v) {
        return multiplyPoint(m, v);
    }
    
#endif