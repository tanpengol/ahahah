#include "transform.h"
#include "config.h"
#include <math.h>
void rotate(const float rotateValue,const VEC3 axis, MAT44 result)
{

	result[0]  = 1 ; result[1]  = 0                 ; result[2]  = 0                  ; result[3]  = 0;
	result[4]  = 0 ; result[5]  = cosf(rotateValue) ; result[6]  = -sinf(rotateValue) ; result[7]  = 0;
	result[8]  = 0 ; result[9]  = sinf(rotateValue) ; result[10] = cosf(rotateValue)  ; result[11] = 0;
	result[12] = 0 ; result[13] = 0                 ; result[14] = 0                  ; result[15] = 1;
	
}

void lookAt(const VEC3  eyepos, const VEC3 center, const VEC3 up, MAT44 result)
{
	VEC3 u,v,w;
	VectorSub(center, eyepos,w);
	normalize(w);
	VectorCross(w, up, u);
	normalize(u);
	VectorCross(u, w, v);
	result[0]  = u[0]; result[1]  = v[0]; result[2]  = w[0]; result[3]  = -VectorDot(u, eyepos);
	result[4]  = u[1]; result[5]  = v[0]; result[6]  = w[0]; result[7]  = -VectorDot(v, eyepos);
	result[8]  = u[2]; result[9]  = v[0]; result[10] = w[0]; 
#ifdef _OGL_
	result[11] =  VectorDot(w, eyepos);
#endif // _OGL_
#ifdef _DIRECTX_
	result[11] = - VectorDot(w, eyepos);
#endif // _OGL_

	result[12] = 0   ; result[13] = 0   ; result[14] = 0   ; result[15] = 1   ;
}

void VectorCross(const VEC3 in1, const VEC3 in2,VEC3 result)
{
	result[0] = in1[1] * in2[2] - in1[2] * in2[1];
	result[1] = in1[2] * in2[0] - in1[0] * in2[2];
	result[2] = in1[0] * in2[1] - in1[1] * in2[0];
}

float VectorDot(const VEC3 in1, const VEC3 in2)
{
	return in1[0] * in2[0] + in1[1] * in2[1] + in1[2] * in2[2];
}

void MatrixmutMAT4(const MAT44 left, const MAT44 right, MAT44 result)
{

	//firstRow
	result[0]  = left[0] * right[0] + left[1] * right[4] + left[2] *  right[8] + left[3] * right[12];
	result[1]  = left[0] * right[1] + left[1] * right[5] + left[2] *  right[9] + left[3] * right[13];
	result[2]  = left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14];
	result[3]  = left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15];

	//secondRow
	result[4]  = left[4] * right[0] + left[5] * right[4] + left[6] *  right[8] + left[7] * right[12];
	result[5]  = left[4] * right[1] + left[5] * right[5] + left[6] *  right[9] + left[7] * right[13];
	result[6]  = left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14];
	result[7]  = left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15];

	//thirdRow
	result[8]  = left[8] * right[0] + left[9] * right[4] + left[10] * right[8]  + left[11] * right[12];
	result[9]  = left[8] * right[1] + left[9] * right[5] + left[10] * right[9]  + left[11] * right[13];
	result[10] = left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14];
	result[11] = left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15];

	//fourthRow
	result[12] = left[12] * right[0] + left[13] * right[4] + left[14] * right[8]  + left[15] * right[12];
	result[13] = left[12] * right[1] + left[13] * right[5] + left[14] * right[9]  + left[15] * right[13];
	result[14] = left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14];
	result[15] = left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15];
}

void perspective(const float fovy, const float aspect, const float zNear, const float zFar, MAT44 result)
{
#ifdef _OGL_
	float tanHalfFovy = tanf(fovy / 2.0f );

	result[0]  = 1.0f / (aspect * tanHalfFovy); result[1]  = 0.0f                ; result[2]   = 0.0f                                   ; result[3]  =  0.0f;
	result[4]  = 0.0f                         ; result[5]  = 1.0f / (tanHalfFovy); result[6]   = 0.0f                                   ; result[7]  =  0.0f;
	result[8]  = 0.0f                         ; result[9]  = 0.0f                ; result[10]  = -(zFar + zNear) / (zFar - zNear)       ; result[11] = -1.0f;
	result[12] = 0.0f                         ; result[13] = 0.0f                ;  result[14] = -(2.0f * zFar * zNear) / (zFar - zNear); result[15] =  0.0f;
#endif // _OGL_	

#ifdef  _DIRECTX_
	float  tanHalfFovy = tanf(fovy / 2.0f);

	result[0] = 1.0f / (aspect * tanHalfFovy); result[1] = 0.0f; result[2] = 0.0f; result[3] = 0.0f;
	result[4] = 0.0f; result[5] = 1.0f / (tanHalfFovy); result[6] = 0.0f; result[7] = 0.0f;
	result[8] = 0.0f; result[9] = 0.0f; result[10] = (zFar + zNear) / (zFar - zNear); result[11] = 1.0f;
	result[12] = 0.0f; result[13] = 0.0f;  result[14] = -( zFar * zNear) / (zFar - zNear); result[15] = 0.0f;

#endif //  _DIRECTX_

	
}

void normalize(VEC3 in)
{
	float len = sqrtf(in[0] * in[0] + in[1] * in[1] + in[2] * in[2]);
	in[0] = in[0] / len;
	in[1] = in[1] / len;
	in[2] = in[2] / len;
 }

void normalizeStayUnchange(const VEC3 in, VEC3 out)
{
	float len = sqrtf(in[0] * in[0] + in[1] * in[1] + in[2] * in[2]);
	out[0] = in[0] / len;
	out[1] = in[1] / len;
	out[2] = in[2] / len;
}

void VectorPlus(const VEC3 in1, const VEC3 in2, VEC3 result)
{
	result[0] = in1[0] + in2[0];
	result[1] = in1[1] + in2[1];
	result[2] = in1[2] + in2[2];
}
void VectorSub(const VEC3 in1, const VEC3 in2, VEC3 result)
{
	result[0] = in1[0] - in2[0];
	result[1] = in1[1] - in2[1];
	result[2] = in1[2] - in2[2];
}


void translate(const float x, const float y, const float z, MAT44  result)
{
	result[3]  = result[0]  * x + result[1]  * y + result[2]  * z + result[3];
	result[7]  = result[4]  * x + result[5]  * y + result[6]  * z + result[7];
	result[11] = result[8]  * x + result[9]  * y + result[10] * z + result[11];
	result[15] = result[12] * x + result[13] * y + result[14] * z + result[15];

}
void translatev(const VEC3 moveVec, MAT44 result)
{
	result[3]  = result[0]  * moveVec[0] + result[1]  * moveVec[1] + result[2]  * moveVec[2] + result[3];
	result[7]  = result[4]  * moveVec[0] + result[5]  * moveVec[1] + result[6]  * moveVec[2] + result[7];
	result[11] = result[8]  * moveVec[0] + result[9]  * moveVec[1] + result[10] * moveVec[2] + result[11];
	result[15] = result[12] * moveVec[0] + result[13] * moveVec[1] + result[14] * moveVec[2] + result[15];
}

void scale(const float scale, MAT44 result)
{
	result[0]  = scale * result[0] ; result[1]  = scale * result[1] ; result[2]  = scale * result[2] ; result[3]  =  result[3];
	result[4]  = scale * result[4] ; result[5]  = scale * result[5] ; result[6]  = scale * result[6] ; result[7]  =  result[7];
	result[8]  = scale * result[8] ; result[9]  = scale * result[9] ; result[10] = scale * result[10]; result[11] =  result[11];
	result[12] = scale * result[12]; result[13] = scale * result[13]; result[14] = scale * result[14]; result[15] =  result[15];
}
void scalev(const VEC3 scale, MAT44 result)
{
	result[0]  = scale[0] * result[0] ; result[1]  = scale[1] * result[1] ; result[2]  = scale[1] * result[2] ; result[3]  = result[3];
	result[4]  = scale[0] * result[4] ; result[5]  = scale[1] * result[5] ; result[6]  = scale[1] * result[6] ; result[7]  = result[7];
	result[8]  = scale[0] * result[8] ; result[9]  = scale[1] * result[9] ; result[10] = scale[1] * result[10]; result[11] = result[11];
	result[12] = scale[0] * result[12]; result[13] = scale[1] * result[13]; result[14] = scale[1] * result[14]; result[15] = result[15];
}