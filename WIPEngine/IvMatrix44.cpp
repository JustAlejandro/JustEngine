#include "IvMatrix44.h"
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
			for (int k = 0; k < DIM; k++) {
				result.mV[i][j] += mV[i][k] * other.mV[k][j];
			}
		}
	}
	return result;
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

float IvMatrix44::CrossProd(const IvMatrix44& other) const {
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
}