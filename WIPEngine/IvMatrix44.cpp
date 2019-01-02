#include "IvMatrix44.h"
#include "Vector4.h"
#define DIM 4

//Matrix-Matrix Addition 4x4
IvMatrix44 IvMatrix44::operator+(const IvMatrix44& other) const {
	IvMatrix44 result;
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			result.mV[i][j] = mV[i][j] + other.mV[i][j];
		}
	}
	return result;
}

//Matrix Multiply 4x4
IvMatrix44 IvMatrix44::operator*(const IvMatrix44& other) const {
	IvMatrix44 result;
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			result.mV[i][j] = 0;
			for (int k = 0; k < DIM; k++) {
				result.mV[i][j] += mV[i][k] * other.mV[k][j];
			}
		}
	}
	return result;
}

//Swaps columns, used for the view2world matrix
IvMatrix44& IvMatrix44::colSwap(int c1, int c2) {
	float t0 = mV[0][c1];
	float t1 = mV[1][c1];
	float t2 = mV[2][c1];
	float t3 = mV[3][c1];
	mV[0][c1] = mV[0][c2];
	mV[1][c1] = mV[1][c2];
	mV[2][c1] = mV[2][c2];
	mV[3][c1] = mV[3][c2];
	mV[0][c2] = t0;
	mV[1][c2] = t1;
	mV[2][c2] = t2;
	mV[3][c2] = t3;
	return *this;
}

//Scales columns, used for the view2world matrix
IvMatrix44& IvMatrix44::colScale(int col, float s) {
	mV[0][col] *= s;
	mV[1][col] *= s;
	mV[2][col] *= s;
	mV[3][col] *= s;

	return *this;
}

//Equals for 4x4 matrix
IvMatrix44& IvMatrix44::operator=(const IvMatrix44& other) {
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			mV[i][j] = other.mV[i][j];
		}
	}
	return *this;
}

float IvMatrix44::Determinant(const IvMatrix44) const {
	float res = mV[0][0] * (mV[1][1] * mV[2][2] - mV[2][1] * mV[1][2]) - mV[0][1] * (mV[1][0] * mV[2][2] - mV[2][0] * mV[1][2]) + mV[0][2] * (mV[1][0] * mV[2][1] - mV[2][0] * mV[1][1]);
	return res;
}

float IvMatrix44::DotProd(const IvMatrix44& other) const {
	float res = 0;
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			res += mV[i][j] * other.mV[i][j];
		}
	}
	return res;
}

//Allows multiplication of 4x4 matrices with Vectors
Vector4 IvMatrix44::operator*(const Vector4 & other) const {
	Vector4 toRet;
	toRet.mV[0] = mV[0][0] * other.mV[0] + mV[0][1] * other.mV[1] + mV[0][2] * other.mV[2] + mV[0][3] * other.mV[3];
	toRet.mV[1] = mV[1][0] * other.mV[0] + mV[1][1] * other.mV[1] + mV[1][2] * other.mV[2] + mV[1][3] * other.mV[3];
	toRet.mV[2] = mV[2][0] * other.mV[0] + mV[2][1] * other.mV[1] + mV[2][2] * other.mV[2] + mV[2][3] * other.mV[3];
	toRet.mV[3] = mV[3][0] * other.mV[0] + mV[3][1] * other.mV[1] + mV[3][2] * other.mV[2] + mV[3][3] * other.mV[3];
	return toRet;
}

IvMatrix44& IvMatrix44::Identity(void) {
	mV[0][0] = 1;
	mV[0][1] = 0;
	mV[0][2] = 0;
	mV[0][3] = 0;
	mV[1][0] = 0;
	mV[1][1] = 1;
	mV[1][2] = 0;
	mV[1][3] = 0;
	mV[2][0] = 0;
	mV[2][1] = 0;
	mV[2][2] = 1;
	mV[2][3] = 0;
	mV[3][0] = 0;
	mV[3][1] = 0;
	mV[3][2] = 0;
	mV[3][3] = 1;
	return *this;
}