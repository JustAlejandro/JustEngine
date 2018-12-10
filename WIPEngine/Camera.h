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
	Transform rotation;
	IvMatrix44 world2view;
	IvMatrix44 perspective;

	//Have to pass pos, and rot parameters for construction
	Camera(float fov = 0, float aspect = 1, float near = 10, float far = 1000, float x = 0, float y = 0, float z = 0, float xr = 0, float yr = 0, float zr = 0);

private:
	IvMatrix44 w2vGen();
	IvMatrix44 perspGen(const float& fov, const float& aspect, const float& near, const float& far);
};





#endif