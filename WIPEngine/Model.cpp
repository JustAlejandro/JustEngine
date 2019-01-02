#include "Model.h"
#include "Transform44.h"
#include <vector>
#include <cmath>
#define ROOTTWOOVERTWO 0.707107

//Constructor for models
Model::Model(float vertices[][3], unsigned int tex[][2], int count, int texIndex, float pos[3], float rot[3], float sca[3]) : position(0, pos[0], pos[1], pos[2]), rotation(2, rot[0], rot[1], rot[2]), scale(1, sca[0], sca[1], sca[2]) {
	//Adds to the vector as long as we have points to add to the model
	for (int i = 0; i < count; i++) {
		points.push_back(Vector4(vertices[i][0], vertices[i][1], vertices[i][2], 1));
		textureCoord.push_back(std::make_pair(tex[i][0],tex[i][1]));
	}
	texture = texIndex;
	pointcount = count;
	axis[0] = rot[0];
	axis[1] = rot[1];
	axis[2] = rot[2];
	mod2world = position.data * rotation.data * scale.data;

	center = pointAvg(points);
	//note, radius actually holds the radius * sqrt(2) / 2 so that way the bounding box has the same radius as the actual object
	//If the bounding box was an approximation of a sqhere then we could just use the actual radius
	radius = maxDist(center, points) * ROOTTWOOVERTWO;

	//This creates the bounding box for culling later
	bounds.push_back(Vector4(center.mV[0],			center.mV[1],		   center.mV[2],		  center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] + radius, center.mV[1] + radius, center.mV[2] + radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] - radius, center.mV[1] - radius, center.mV[2] + radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] + radius, center.mV[1] - radius, center.mV[2] + radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] - radius, center.mV[1] + radius, center.mV[2] + radius, center.mV[3]));

	bounds.push_back(Vector4(center.mV[0] + radius, center.mV[1] + radius, center.mV[2] - radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] - radius, center.mV[1] - radius, center.mV[2] - radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] + radius, center.mV[1] - radius, center.mV[2] - radius, center.mV[3]));
	bounds.push_back(Vector4(center.mV[0] - radius, center.mV[1] + radius, center.mV[2] - radius, center.mV[3]));

}

//Copy Assignment
Model Model::operator=(const Model & other) {
	texture = other.texture;
	pointcount = other.pointcount;
	position = other.position;
	scale = other.scale;
	rotation = other.rotation;
	points.clear();
	textureCoord.clear();
	for (int i = 0; i < other.points.size(); i++) {
		points.push_back(Vector4(other.points[i]));
		textureCoord.push_back(other.textureCoord[i]);
	}
	axis[0] = other.axis[0];
	axis[1] = other.axis[1];
	axis[2] = other.axis[2];
	mod2world = position.data * rotation.data * scale.data;

	center = other.center;
	radius = other.radius;
	bounds.clear();
	for (int i = 0; i < other.bounds.size(); i++) {
		bounds.push_back(Vector4(other.bounds[i]));
	}
	return *this;
}

//Copy Declaration
Model::Model(const Model& other) : position(other.position),rotation(other.rotation),scale(other.scale) {
	texture = other.texture;
	axis[0] = other.axis[0];
	axis[1] = other.axis[1];
	axis[2] = other.axis[2];
	mod2world = other.mod2world;
	points.clear();
	textureCoord.clear();
	for (int i = 0; i < other.pointcount; i++) {
		points.push_back(other.points[i]);
		textureCoord.push_back(other.textureCoord[i]);
	}
	pointcount = other.pointcount;

	center = other.center;
	radius = other.radius;
	bounds.clear();
	for (int i = 0; i < other.bounds.size(); i++) {
		bounds.push_back(Vector4(other.bounds[i]));
	}
}

//Changes position to a new spot
void Model::warp(float x, float y, float z) {
	position.data.mV[0][3] = x;
	position.data.mV[1][3] = y;
	position.data.mV[2][3] = z;
	mod2world = position.data * rotation.data * scale.data;
}

//Moves Model by specified x,y,z
void Model::move(float x, float y, float z) {
	position.data.mV[0][3] += x;
	position.data.mV[1][3] += y;
	position.data.mV[2][3] += z;
	mod2world = position.data * rotation.data * scale.data;
}

//Changes the scale of the model
void Model::size(float x, float y, float z) {
	scale.data.mV[0][0] = x;
	scale.data.mV[1][1] = y;
	scale.data.mV[2][2] = z;
	mod2world = position.data * rotation.data * scale.data;
}

//Adds to current scale
void Model::grow(float x, float y, float z) {
	scale.data.mV[0][0] += x;
	scale.data.mV[1][1] += y;
	scale.data.mV[2][2] += z;
	mod2world = position.data * rotation.data * scale.data;
}

//Changes the rotation of the model
void Model::rotate(float x, float y, float z) {
	axis[0] = x;
	axis[1] = y;
	axis[2] = z;
	rotation = Transform(2, x, y, z);
	mod2world = position.data * rotation.data * scale.data;
}

//Adds to the current rotation
void Model::spin(float x, float y, float z) {
	axis[0] += x;
	axis[1] += y;
	axis[2] += z;
	rotation = Transform(2, axis[0], axis[1], axis[2]);
	mod2world = position.data * rotation.data * scale.data;
}



//*************************************************HELPER FUNCTIONS **********************************************************

//pointAvg: finds the average point from a vector of points
Vector4 pointAvg(std::vector<Vector4> points) {
	float x = 0;
	float y = 0;
	float z = 0;
	for (int i = 0; i < points.size(); i++) {
		x += points[i].mV[0];
		y += points[i].mV[1];
		z += points[i].mV[2];
	}
	x = x / points.size();
	y = y / points.size();
	z = z / points.size();
	return Vector4(x, y, z, 1);
}

//maxDist: finds the largest distance between a Vector4 and a vector of Vector4s
float maxDist(Vector4 center, std::vector<Vector4> points) {
	float dist = 0;
	float compare = 0;
	for (int i = 0; i < points.size(); i++) {
		compare = distance(center, points[i]);
		if (compare > dist) {
			dist = compare;
		}
	}
	return dist;
}

//Finds the distance between 2 Vector4's (assuming that the w component is irrelevant since it holds whether it's a point or vector)
float distance(Vector4 a, Vector4 b) {
	return sqrt(pow((a.mV[0] - b.mV[0]), 2) + pow((a.mV[1] - b.mV[1]), 2) + pow((a.mV[2] - b.mV[2]), 2));
}