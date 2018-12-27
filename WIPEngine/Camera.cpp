#include "Camera.h"
#include "Transform44.h"
#include "IvMatrix44.h"
#include <cmath>
#define PI 3.14159265

//Helper Prototype
void transposeUpperRight(IvMatrix44& mat);

/*Default constructor holds all the data for the Camera object
	Creates a default perspective matrix.
	Here we'll call the contructor for the Tranform objects in Camera class
*/
Camera::Camera(float fov, float aspect, float near, float far, float x, float y, float z, float xr, float yr, float zr) : position(0, x, y, z), rotation(2, xr, yr, zr) {
	view2world = v2wGen();
	world2view = w2vGen();
	perspective = perspGen(fov, aspect, near, far);
	axis[0] = xr;
	axis[1] = yr;
	axis[2] = zr;
}

//Moves the camera by adding to position rather than overriding it
void Camera::move(float x, float y, float z) {
	position.data.mV[0][3] += x;
	position.data.mV[1][3] += y;
	position.data.mV[2][3] += z;
}

//Changes the rotation of the camera to x,y,z angles
//Note: should probably add in a way to increase the rotation rather than completely change it
void Camera::rotate(float x, float y, float z) {
	axis[0] = x;
	axis[1] = y;
	axis[2] = z;
	rotation = Transform(2, x, y, z);
}

//Repositions the camera entirely rather than moving
void Camera::warp(float x, float y, float z) {
	position.data.mV[0][3] = x;
	position.data.mV[1][3] = y;
	position.data.mV[2][3] = z;
}

//Generates the world2view matrix from the view2world matrix
IvMatrix44 Camera::w2vGen() {
	IvMatrix44 world2view = view2world;
	//Form of world to view is inverse of upper right triangular 3x3 which is the same as transpose since it's an orthogonal matrix
	transposeUpperRight(world2view);
	world2view.mV[0][3] = world2view.mV[0][0] * world2view.mV[0][3] + world2view.mV[0][1] * world2view.mV[1][3] + world2view.mV[0][2] * world2view.mV[2][3];
	world2view.mV[1][3] = world2view.mV[1][0] * world2view.mV[0][3] + world2view.mV[1][1] * world2view.mV[1][3] + world2view.mV[1][2] * world2view.mV[2][3];
	world2view.mV[2][3] = world2view.mV[2][0] * world2view.mV[0][3] + world2view.mV[2][1] * world2view.mV[1][3] + world2view.mV[2][2] * world2view.mV[2][3];
	
	return world2view;
}

//Generates the veiw2world matrix based on the position and rotation of the camera
IvMatrix44 Camera::v2wGen() {
	//Setup the rotation matrix to be the upper 3x3
	IvMatrix44 view2world = rotation.data;
	//Setup the position to be the rightmost column
	view2world.mV[0][3] = position.data.mV[0][3];
	view2world.mV[1][3] = position.data.mV[1][3];
	view2world.mV[2][3] = position.data.mV[2][3];
	view2world.colSwap(0, 2);
	view2world.colSwap(0, 1);
	view2world.colScale(0, -1);
	view2world.colScale(2, -1);

	return view2world;
}

//Generates the perspective matrix
//	AKA the specific rendering skew that our engine will use, but we can change to other schemes
IvMatrix44 Camera::perspGen(const float& fov, const float& aspect, const float& near, const float& far) {
	float d = 1.0 / tan((fov / 2.0) * PI / 180.0);
	IvMatrix44 persp;
	//Regular Perspective Matrix
	persp.Identity();
	persp.mV[0][0] = d / aspect;
	persp.mV[1][1] = d;
	persp.mV[2][2] = (near + far) / (near - far);
	persp.mV[2][3] = (2 * near * far) / (near - far);
	persp.mV[3][2] = -1;
	persp.mV[3][3] = 0;
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