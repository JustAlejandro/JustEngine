#include "Transform44.h"
#include <cmath>
#define DIM 4
#define PI 3.14159265358979323846

/*This is our default constructor that will make a Transform matrix
	Input: Type of Matrix, x,y,z parameters
		Type:	0 = Translate
				1 = Scale
				2 = Rotate
				Might be a better way to do this, but this uses the least amount of data/CPU time
	This implementation defaults to the identity matrix if it's unsure there is no type selected
*/
Transform::Transform(int type, float x, float y, float z) {
	switch (type)
	{
	case 0:
		Translate(x, y, z);
		break;
	case 1:
		Scale(x, y, z);
		break;
	case 2:
		Rotate(x, y, z);
		break;
	default:
		break;
	}
	return;
}

//Copy constructor
Transform::Transform(const Transform & other) {
	data = other.data;
}

//Creates a default translation matrix
Transform& Transform::Translate(float x, float y, float z) {
	data.Identity();
	data.mV[0][DIM - 1] = x;
	data.mV[1][DIM - 1] = y;
	data.mV[2][DIM - 1] = z;
	return *this;
}

//Creates the default matrix that scales objects
Transform& Transform::Scale(float x, float y, float z) {
	data.Identity();
	data.mV[0][0] = x;
	data.mV[1][1] = y;
	data.mV[2][2] = z;
	return *this;
}

//Creates xyz rotation matrix
Transform& Transform::Rotate(float x, float y, float z) {
	float cx = cos(x * PI / 180.0);
	float cy = cos(y * PI / 180.0);
	float cz = cos(z * PI / 180.0);
	float sx = sin(x * PI / 180.0);
	float sy = sin(y * PI / 180.0);
	float sz = sin(z * PI / 180.0);

	data.mV[0][0] = cy * cz;
	data.mV[0][1] = -1 * cy * sz;
	data.mV[0][2] = sy;
	data.mV[0][3] = 0;
	data.mV[1][0] = sx * sy * cz + cx * sz;
	data.mV[1][1] = -1 * sx * sy * sz + cx * cz;
	data.mV[1][2] = -1 * sx * cy;
	data.mV[1][3] = 0;
	data.mV[2][0] = -1 * cx * sy * cz + sx * sz;
	data.mV[2][1] = cx * sy * sz + sx * cz;
	data.mV[2][2] = cx * cy;
	data.mV[2][3] = 0;
	data.mV[3][0] = 0;
	data.mV[3][1] = 0;
	data.mV[3][2] = 0;
	data.mV[3][3] = 1;
	return *this;
}

//Calls basic 4x4 multiplication
Transform Transform::operator*(const Transform& other) const {
	Transform toRet;
	toRet.data = data * other.data;
	return toRet;
}

Transform & Transform::operator=(const Transform & other) {
	data = other.data;
	return *this;
}
