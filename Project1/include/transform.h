#pragma once

#ifndef _OGL_TRANSFORM_H_
#define _OGL_TRANSFORM_H_

typedef float MAT44[16];
typedef float  VEC4[4] ;
typedef float  VEC3[3] ;

enum ROTATE_TYPE
{
	ROTATE_DEGREES = 1,
	ROTATE_RADIANS = 2,
};

void rotate(const float rotateValue, const VEC3 axis, MAT44 result);

void translate(const float x,const float y,const float z,MAT44 result);
void translatev(const VEC3 moveVec, MAT44 result);
void scale(const float scale,MAT44 result);
void scalev(const VEC3 scale, MAT44 result);
void perspective(const float fovy,const float aspect,const float zNear,const float zFar,MAT44 result);

void lookAt(const VEC3 eyepos, const VEC3 center,const VEC3 up,MAT44 result);

void normalize(VEC3 in);
void normalizeStayUnchange(const VEC3 in,VEC3 out);


//vector cross product
void VectorCross(const VEC3 in1, const VEC3 in2, VEC3 result);
float VectorDot(const VEC3 in1, const VEC3 in2);
void VectorPlus(const VEC3 in1, const VEC3 in2, VEC3 result);
void VectorSub(const VEC3 in1, const VEC3 in2, VEC3 result);

void MatrixmutMAT4(const MAT44 left, const MAT44 right, MAT44 result);

#endif // !_OGL_TRANSFORM_H_

