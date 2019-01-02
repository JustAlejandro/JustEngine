#include "Vector4.h"
#include <math.h>

//Simple initializer for Vectors
Vector4::Vector4(float x, float y, float z, float w) {
	mV[0] = x;
	mV[1] = y;
	mV[2] = z;
	mV[3] = w;
}

//Allows for us to add 2 vectors
Vector4 Vector4::operator+(const Vector4 & other) const {
	return Vector4(mV[0] + other.mV[0], mV[1] + other.mV[1], mV[2] + other.mV[2], mV[3] + other.mV[3]);
}

Vector4 Vector4::operator-(const Vector4 & other) const {
	return Vector4(mV[0] - other.mV[0], mV[1] - other.mV[1], mV[2] - other.mV[2], mV[3] - other.mV[3]);
}

Vector4 Vector4::operator/(const float div) const {
	return Vector4(mV[0] / div, mV[1] / div, mV[2] / div, mV[3] / div);
}

Vector4 Vector4::operator*(const float div) const {
	return Vector4(mV[0] * div, mV[1] * div, mV[2] * div, mV[3] * div);
}

//Divides all elements by last element
Vector4 & Vector4::homDiv() {
	mV[0] = mV[0] / mV[3];
	mV[1] = mV[1] / mV[3];
	mV[2] = mV[2] / mV[3];
	mV[3] = mV[3] / mV[3];
	return *this;
}