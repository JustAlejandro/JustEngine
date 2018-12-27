#ifndef __Model__h__
#define __Model__h__
#include "Transform44.h"
#include "IvMatrix44.h"
#include <vector>
#include <cmath>

/*Class Model holds a series of points
	Contains:
		Position
		Scale
		Rotation orientation
		Rotation matrix
		Model to World space matrix
			DRAWBACK: This will be updated in every operation so that we can't forget about it, may make it part of the render pipeline for efficiency's sake
		Vector of Vector4 points
*/
class Model {
public:
	int pointcount;
	Transform position;
	Transform scale;
	//Holds the actual values of the rotation, realized that we need to keep these seperate from our rotation matrix in order to modify it
	float axis[3];
	Transform rotation;
	IvMatrix44 mod2world;
	std::vector<Vector4> points;
	//These parameters are used for culling
	Vector4 center;
	float radius;
	std::vector<Vector4> bounds;

	Model(float vertices[][4], int count, float pos[3], float rot[3], float sca[3]);
	Model operator=(const Model& other);
	Model(const Model& other);
	//Changes position to new 3 coordinates
	void warp(float x, float y, float z);
	//Adds position to new 3 coordinates
	void move(float x, float y, float z);
	//Changes size instantaneously
	void size(float x, float y, float z);
	//Adds or subtracts from current size
	void grow(float x, float y, float z);
	//Changes x,y,z degrees rotation
	void rotate(float x, float y, float z);
	//Adds or subtracts from current rotation
	void spin(float x, float y, float z);
};

Vector4 pointAvg(std::vector<Vector4> points);
float maxDist(Vector4 center, std::vector<Vector4> points);
float distance(Vector4 a, Vector4 b);

#endif