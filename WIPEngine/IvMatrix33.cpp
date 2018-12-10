#include "IvMatrix33.h"
#define DIM 3

//Matrix-Matrix Addition 3x3
IvMatrix33
IvMatrix33::operator+(const IvMatrix33& other) const {
	IvMatrix33 result;
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			result.mV[i][j] = mV[i][j] + other.mV[i][j];
		}
	}
	return result;
}

//Matrix Multiply 3x3
IvMatrix33
IvMatrix33::operator*(const IvMatrix33& other) const {
	IvMatrix33 result;
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				result.mV[i][j] += mV[i][k] * other.mV[k][j];
			}
		}
	}
	return result;
}

//Equals for 3x3 matrix
IvMatrix33&
IvMatrix33::operator=(const IvMatrix33& other) {
	for (int i = 0; i < DIM; i++)
	{
		for (int j = 0; j < DIM; j++)
		{
			mV[i][j] = other.mV[i][j];
		}
	}
	return *this;
}

float
IvMatrix33::Determinant(const IvMatrix33) const {
	float res = mV[0][0] * (mV[1][1]*mV[2][2] - mV[2][1]*mV[1][2]) - mV[0][1] * (mV[1][0]*mV[2][2] - mV[2][0]*mV[1][2]) + mV[0][2] * (mV[1][0]*mV[2][1] - mV[2][0]*mV[1][1]);
	return res;
}

float
IvMatrix33::CrossProd(const IvMatrix33& other) const {
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