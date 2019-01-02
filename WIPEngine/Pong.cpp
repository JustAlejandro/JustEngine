#include "Pong.h"
#include "Render.h"
#include <chrono>
#include <SDL.h>
#include "Textures.h"


//Globals to define the speed of player characters
float speed1 = CHARSPEED;
float speed2 = CHARSPEED;
//Globals that the constructor for pong relies on
float verts[14][3] = { {-1,1,3}, {1,1,3}, {-1,-1,3}, {1,-1,3}, {1,-1,-3}, {1,1,3}, {1,1,-3}, {-1,1,3}, {-1,1,-3}, {-1,-1,3}, {-1,-1,-3}, {1,-1,-3}, {-1,1,-3}, {1,1,-3} };
unsigned int texs[14][2] = {{T,0}, {0,0},     {T,T},    {0,T},     {0,0},   {0,0},	  {T,0},    {0,T},     {T,T},     {0,0},      {T,0},     {T,T},     {0,0},    {0,T} };
float rot[3] = { 0,0,0 };
float pos[3] = { 16,18,0 };
float sca[3] = { 1,1,1 };
float pos2[3] = { 16,-18,0 };
float ball[14][3] = { {-RAD,RAD,RAD}, {RAD,RAD,RAD}, {-RAD,-RAD,RAD}, {RAD,-RAD,RAD}, {RAD,-RAD,-RAD}, {RAD,RAD,RAD}, {RAD,RAD,-RAD}, {-RAD,RAD,RAD}, {-RAD,RAD,-RAD}, {-RAD,-RAD,RAD}, {-RAD,-RAD,-RAD}, {RAD,-RAD,-RAD}, {-RAD,RAD,-RAD}, {RAD,RAD,-RAD} };
float ballpos[3] = { 16,0,0 };
float ballsca[3] = { 1,1,1 };
float walls[8][3] = { {-X,Y,-Z},{-X,-Y,-Z},{0,Y,-Z},{0,-Y,-Z},{0,Y,Z},{0,-Y,Z},{-X,Y,Z},{-X,-Y,Z} };
unsigned int waltex[8][2] = { {0,0}, {T,0},   {0,T},    {T,T},  {0,0},   {T,0},   {0,T},    {T,T} };
float poswall[3] = { 18,0,0 };
//Default constructor for Match takes in an array of Models and a count of how many Models
Model scene[] = { Model(verts, texs, 14, BLUE, pos, rot, sca), Model(verts, texs, 14, RED, pos2,rot,sca), Model(walls,waltex,8, BRICK, poswall,rot,sca), Model(ball,texs,14, METAL, ballpos,rot,ballsca) };

//Used for depth testing and will be cleared when we destroy the Pong object
float *depth = new float[1920 / 2 * 1080 / 2];

//Declares Pong and finds if we're in sightseeing mode, also calls the constructor for class Match
Pong::Pong(bool sightseeing) : game(scene, 4) {
	this->sightseeing = sightseeing;
	forcequit = 0;
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pong - Just Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
	target = SDL_GetWindowSurface(window);
	keyboard = SDL_GetKeyboardState(NULL);
	balldir[0] = 0;
	balldir[1] = STARTSPEED;
	balldir[2] = 0;
	status[0] = 1;
	status[1] = 0;
	status[2] = 0;
	game.moveCamera(-8, 0, 0);
	if (sightseeing) {
		balldir[1] = 0;
	}
	SDL_RaiseWindow(window);
}

Pong::~Pong() {
	delete depth;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//Updates positions of all objects as well as checks hitboxes
bool Pong::play() {
	//FPS timer
	auto start = std::chrono::steady_clock::now();
	//Checks if player has manually quit
	if (!keyboardHandler()) {
		return 0;
	}
	p1Hit(game, balldir);
	p2Hit(game, balldir);
	wallHit(game, balldir);
	ballSpin(game, balldir);
	game.moveModel(BALL, balldir[0], balldir[1], balldir[2]);
	gameCheck();
	renderUpdate(target, depth, game);
	SDL_UpdateWindowSurface(window);
	if (status[2] == 1 || status[1] == 1) {
		SDL_HideWindow(window);
	}
	//Waits ensuring the game doesn't run over FPS target
	auto stop = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> difference = (stop - start);
	while (difference.count() < FRAMETIME) {
		stop = std::chrono::steady_clock::now();
		difference = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start);
	}
	return status[0];
}

bool Pong::Winner() const {
	return status[2];
}

void Pong::gameCheck() {
	//P2 Win
	if (game.modelY(BALL) > game.modelY(P1) + 3) {
		status[0] = 0;
		status[1] = 0;
		status[2] = 1;
	}
	//P1 Win
	if (game.modelY(BALL) < game.modelY(P2) - 3) {
		status[0] = 0;
		status[1] = 1;
		status[2] = 0;
	}

}

void Pong::playerMove(int player, float x, float y, float z) {
	//Checks downward movement vs the wall
	if ((game.modelZ(player) - 3 > game.modelZ(WALL) - Z) && z < 0) {
		game.moveModel(player, x, y, z);
	}
	if ((game.modelZ(player) - 3 < game.modelZ(WALL) - Z)) {
		game.warpModel(player, game.modelX(player), game.modelY(player), -Z + 3.1);
	}

	//Checks upward movement vs the wall
	if ((game.modelZ(player) + 3 < game.modelZ(WALL) + Z) && z > 0) {
		game.moveModel(player, x, y, z);
	}
	if ((game.modelZ(player) + 3 > game.modelZ(WALL) + Z)) {
		game.warpModel(player, game.modelX(player), game.modelY(player), + Z - 3.1);
	}
}

/*Reset ball position and velocity as well as the status vector
  Also brings back window holding the game view */
void Pong::Reset() {
	SDLReset();

	balldir[0] = 0;
	balldir[1] = STARTSPEED;
	balldir[2] = 0;
	status[0] = 1;
	status[1] = 0;
	status[2] = 0;
	game.warpModel(BALL, 16, 0, 0);
	game.warpModel(P1, game.modelX(P1), game.modelY(P1), 0);
	game.warpModel(P2, game.modelX(P2), game.modelY(P2), 0);
	SDL_ShowWindow(window);
	SDL_RaiseWindow(window);
}

//Basic hit detection, checking to see if the model's bounds have contact with another model's
void p1Hit(Match& game, float balldir[3]) {
	if (game.modelY(BALL) + RAD * 1.25 > game.modelY(P1) - 1 && game.modelY(BALL) - RAD < game.modelY(P1) - 1) {
		if (game.modelZ(BALL) > game.modelZ(P1) - 3.25 && game.modelZ(BALL) < game.modelZ(P1) + 3.25) {
			if (game.modelX(BALL) > game.modelX(P1) - 1 && game.modelX(BALL) < game.modelX(P1) + 1) {
				balldir[1] = -1 * abs(balldir[1]) - INCREASE;
				balldir[2] = speedGen();
				return;
			}
		}
	}
}
void p2Hit(Match& game, float balldir[3]) {
	if (game.modelY(BALL) - RAD * 1.25 < game.modelY(P2) + 1 && game.modelY(BALL) + RAD > game.modelY(P2) + 1) {
		if (game.modelZ(BALL) > game.modelZ(P2) - 3.25 && game.modelZ(BALL) < game.modelZ(P2) + 3.25) {
			if (game.modelX(BALL) > game.modelX(P2) - 1 && game.modelX(BALL) < game.modelX(P2) + 1) {
				balldir[1] = abs(balldir[1]) + INCREASE;
				balldir[2] = speedGen();
			}
		}
	}
}
void wallHit(Match& game, float balldir[3]) {
	if (game.modelY(BALL) > game.modelY(WALL) - Y - 1 && game.modelY(BALL) < game.modelY(WALL) + Y + 1) {
		if ((game.modelZ(BALL) > game.modelZ(WALL) - Z - 2* RAD && game.modelZ(BALL) < game.modelZ(WALL) - Z + RAD * 1.25) || (game.modelZ(BALL) > game.modelZ(WALL) + Z - RAD * 1.25 && game.modelZ(BALL) < game.modelZ(WALL) + Z + RAD * 2)) {
			if (game.modelX(BALL) > game.modelX(WALL) - X - 1 && game.modelX(BALL) < game.modelX(WALL) + X + 1) {
				balldir[2] = balldir[2] * -1;
			}
		}
	}

	if (game.modelY(BALL) > game.modelY(WALL) - Y - 1 && game.modelY(BALL) < game.modelY(WALL) + Y + 1) {
		if ((game.modelZ(BALL) > game.modelZ(WALL) - Z - RAD && game.modelZ(BALL) < game.modelZ(WALL) + Z + RAD)) {
			if ((game.modelX(BALL) > game.modelX(WALL) - X - 1 && game.modelX(BALL) < game.modelX(WALL) - X + 1) || (game.modelX(BALL) > game.modelX(WALL) - 1 && game.modelX(BALL) < game.modelX(WALL) + 1)) {
				balldir[0] = balldir[0] * -1;
			}
		}
	}
}

//Fun for looks, modifies the spin of the ball according to it's current velocity in each direction
void ballSpin(Match& game, float balldir[3]) {
	game.spinModel(BALL, balldir[1] * SPIN, balldir[2] * SPIN, 0);
}

//Generates our speed to launch back the ball after hitting a paddle
float speedGen() {
	return (rand() % (int)(UPSPEED * 2 * 1000) - UPSPEED * 1000) * .001;
}

//This restarts SDL in its entirety, have to do this because otherwise keyboard inputs take about however long the game window is in the background for to clear pending inputs
void Pong::SDLReset() {
	SDL_DestroyWindow(window);
	SDL_Quit();
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pong - Just Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
	target = SDL_GetWindowSurface(window);
	keyboard = SDL_GetKeyboardState(NULL);
}

//Using SDL's basic system for polling the status of the entire keyboard at once we can detect multiple keypresses
bool Pong::keyboardHandler() {
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_PumpEvents();
	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		forcequit = 1;
		SDL_HideWindow(window);
		return 0;
	}
	if (keyboard[SDL_SCANCODE_LSHIFT]) {
		speed1 = CHARSPEED * 1.5;
	}
	else {
		speed1 = CHARSPEED;
	}
	if (keyboard[SDL_SCANCODE_KP_0]) {
		speed2 = CHARSPEED * 1.5;
	}
	else {
		speed2 = CHARSPEED;
	}
	if (keyboard[SDL_SCANCODE_W]) {
		playerMove(P1, 0, 0, speed1);
	}
	if (keyboard[SDL_SCANCODE_S]) {
		playerMove(P1, 0, 0, -1 * speed1);
	}
	if (keyboard[SDL_SCANCODE_UP]) {
		playerMove(P2, 0, 0, speed2);
	}
	if (keyboard[SDL_SCANCODE_DOWN]) {
		playerMove(P2, 0, 0, -1 * speed2);
	}
	//Camera controls in sighseeing mode
	if (sightseeing) {
		if (keyboard[SDL_SCANCODE_KP_8]) {
			game.moveCamera(0, 0, 1);
		}
		if (keyboard[SDL_SCANCODE_KP_2]) {
			game.moveCamera(0, 0, -1);
		}
		if (keyboard[SDL_SCANCODE_KP_6]) {
			game.moveCamera(0, -1, 0);
		}
		if (keyboard[SDL_SCANCODE_KP_4]) {
			game.moveCamera(0, 1, 0);
		}
		if (keyboard[SDL_SCANCODE_KP_7]) {
			game.spinCamera(0, 0, 1);
		}
		if (keyboard[SDL_SCANCODE_KP_9]) {
			game.spinCamera(0, 0, -1);
		}
		if (keyboard[SDL_SCANCODE_KP_PLUS]) {
			game.moveCamera(1, 0, 0);
		}
		if (keyboard[SDL_SCANCODE_KP_MINUS]) {
			game.moveCamera(-1, 0, 0);
		}
	}

	return 1;
}
