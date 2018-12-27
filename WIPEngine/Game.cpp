#include "Game.h"
#include <vector>

//Holds all the models that are contained in the scene as well as a specific model for the player
Match::Match(Model user, Model * world, int modelcount) : player(user) {
	player = user;
	sceneCount = modelcount;
	for (int i = 0; i < modelcount; i++) {
		scene.push_back(Model(world[i]));
	}
}
