#include "Camera.h"
#include "Transform44.h"
#include "IvMatrix44.h"
#include <cmath>

//Helper Prototype
void transposeUpperRight(IvMatrix44& mat);

/*Default constructor holds all the data for the Camera object
	Creates a default perspective matrix.
	Here we'll call the contructor for the Tranform objects in Camera class
*/
Camera::Camera(float fov, float aspect, float near, float far, float x, float y, float z, float xr, float yr, float zr) : position(0, x, y, z), rotation(2, xr, yr, zr) {
	world2view = w2vGen();
	perspective = perspGen(fov, aspect, near, far);
}

IvMatrix44 Camera::w2vGen() {
	//Setup the rotation matrix to be the upper 3x3
	IvMatrix44 world2view = rotation.data;
	//Setup the position to be the rightmost column
	world2view.mV[0][3] = position.data.mV[0][3];
	world2view.mV[1][3] = position.data.mV[1][3];
	world2view.mV[2][3] = position.data.mV[2][3];
	world2view.colSwap(0, 2);
	world2view.colSwap(0, 1);
	world2view.colScale(0, -1);
	world2view.colScale(2, -1);
	
	//Form of world to view is inverse of upper right triangular 3x3 which is the same as transpose since it's an orthogonal matrix
	transposeUpperRight(world2view);
	world2view.mV[0][3] = world2view.mV[0][0] * world2view.mV[0][3] + world2view.mV[0][1] * world2view.mV[1][3] + world2view.mV[0][2] * world2view.mV[2][3];
	world2view.mV[1][3] = world2view.mV[1][0] * world2view.mV[0][3] + world2view.mV[1][1] * world2view.mV[1][3] + world2view.mV[1][2] * world2view.mV[2][3];
	world2view.mV[2][3] = world2view.mV[2][0] * world2view.mV[0][3] + world2view.mV[2][1] * world2view.mV[1][3] + world2view.mV[2][2] * world2view.mV[2][3];
	
	return world2view;
}

IvMatrix44 Camera::perspGen(const float& fov, const float& aspect, const float& near, const float& far) {
	float d = 1 / tan(fov / 2);
	IvMatrix44 persp;
	//Regular Perspective Matrix
	persp.Identity();
	persp.mV[0][0] = d / aspect;
	persp.mV[1][1] = d;
	persp.mV[2][2] = (near + far) / (near - far);
	persp.mV[2][3] = (2 * near * far) / (near - far);
	persp.mV[3][2] = -1;
	return persp;
}

//Helper function used in calculating the world2view matrix, same as upper left transpose for orthogonal 3x3
void transposeUpperRight(IvMatrix44& mat) {
	float temp = mat.mV[0][1];
	mat.mV[0][1] = mat.mV[1][0];
	mat.mV[1][0] = temp;

	temp = mat.mV[0][2];
	mat.mV[0][2] = mat.mV[2][0];
	mat.mV[2][0] = temp;

	temp = mat.mV[1][2];
	mat.mV[1][2] = mat.mV[2][1];
	mat.mV[2][1] = temp;
}