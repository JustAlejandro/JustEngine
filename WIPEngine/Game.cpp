#include "Game.h"
#include <vector>

//Holds all the models that are contained in the scene as well as a specific model for the player
Match::Match(Model * world, int modelcount) {
	sceneCount = modelcount;
	for (int i = 0; i < modelcount; i++) {
		scene.push_back(Model(world[i]));
	}
}

void Match::moveModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].move(x, y, z);
	}
}

void Match::warpModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].warp(x, y, z);
	}
}

void Match::spinModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].spin(x, y, z);
	}
}

void Match::rotateModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].rotate(x, y, z);
	}
}

void Match::sizeModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].size(x, y, z);
	}
}

void Match::growModel(int index, float x, float y, float z) {
	if (index < scene.size()) {
		scene[index].grow(x, y, z);
	}
}

void Match::moveCamera(float x, float y, float z) {
	POV.move(x, y, z);
}

void Match::spinCamera(float x, float y, float z) {
	POV.spin(x, y, z);
}

float Match::modelX(int index) {
	return scene[index].position.data.mV[0][3];
}

float Match::modelY(int index) {
	return scene[index].position.data.mV[1][3];
}

float Match::modelZ(int index) {
	return scene[index].position.data.mV[2][3];
}

