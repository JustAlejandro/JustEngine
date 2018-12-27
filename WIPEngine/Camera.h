#ifndef __Camera__h__
#define __Camera__h__
#include "Transform44.h"
#include "IvMatrix44.h"
/*Defines the class Camera to hold camera objects as well as type, rotation, and position
	NOTE: All are done through the right handed system where -z is the view direction of the camera
			same as openGL
*/
class Camera {
public:
	Transform position;
	float axis[3];
	Transform rotation;
	IvMatrix44 world2view;
	IvMatrix44 view2world;
	IvMatrix44 perspective;

	//Have to pass pos, and rot parameters for construction
	Camera(float fov = 90, float aspect = 1920.0/1080.0, float near = 1, float far = 10, float x = 0, float y = 0, float z = 0, float xr = 0, float yr = 0, float zr = 0);
	void move(float x, float y, float z);
	void rotate(float x, float y, float z);
	void warp(float x, float y, float z);

private:
	IvMatrix44 w2vGen();
	IvMatrix44 v2wGen();
	IvMatrix44 perspGen(const float& fov, const float& aspect, const float& near, const float& far);
};





#endif