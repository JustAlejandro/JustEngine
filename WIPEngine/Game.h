#ifndef __Game__h__
#define __Game__h__
#include "Camera.h"
#include "Model.h"

/* Match holds all the different elements needed to run a simple game
	Contains:
		Camera to render
		Number of models
		Vector holding all the non-player models

	NOTE: For added functionality, may add in another layer above Model
		  to hold characters made of multiple models allowing for animation
*/
class Match {
public:
	Camera POV;
	int sceneCount;
	std::vector<Model> scene;
	Match(Model *world, int modelcount);

	void moveModel(int index, float x, float y, float z);

	void warpModel(int index, float x, float y, float z);

	void spinModel(int index, float x, float y, float z);

	void rotateModel(int index, float x, float y, float z);

	void sizeModel(int index, float x, float y, float z);

	void growModel(int index, float x, float y, float z);

	void moveCamera(float x, float y, float z);

	void spinCamera(float x, float y, float z);

	float modelX(int index);

	float modelY(int index);

	float modelZ(int index);
};

#endif