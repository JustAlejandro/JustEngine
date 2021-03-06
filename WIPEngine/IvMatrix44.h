#ifndef __IvMatrix44__h__
#define __IvMatrix44__h__
#include "Vector4.h"

/*Defines the class IvMatrix44 as a 4x4 matrix
*/
class IvMatrix44 {
public:
	float mV[4][4];

	IvMatrix44& Identity(void);
	IvMatrix44 operator+(const IvMatrix44& other) const;
	IvMatrix44& operator=(const IvMatrix44& other);
	IvMatrix44 operator*(const IvMatrix44& other) const;
	IvMatrix44& colSwap(int c1, int c2);
	IvMatrix44& colScale(int col, float s);
	float Determinant(const IvMatrix44) const;
	float DotProd(const IvMatrix44& other) const;
	Vector4 operator*(const Vector4& other) const;

};

#endif