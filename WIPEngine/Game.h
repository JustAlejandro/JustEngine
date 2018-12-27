#ifndef __Game__h__
#define __Game__h__
#include <SDL.h>
#include "Camera.h"
#include "Model.h"

/* Match holds all the different elements needed to run a simple game
	Contains:
		Camera to render
		Model that is the player
		Number of non-player models
		Vector holding all the non-player models

	NOTE: For added functionality, may add in another layer above Model
		  to hold characters made of multiple models allowing for animation
*/
class Match {
public:
	Camera POV;
	Model player;
	int sceneCount;
	std::vector<Model> scene;
	Match(Model user, Model *world, int modelcount);
};

#endif