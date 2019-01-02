#ifndef __Pong__h__
#define __Pong__h__
#include <SDL.h>
#include "Game.h"
#include "Render.h"
//Wall Dimensions
#define X 10
#define Y 22
#define Z 13
//Indexes of objects
#define P1 0
#define P2 1
#define WALL 2
#define BALL 3
#define RAD .5 //Radius of ball
#define SPIN 20 //Spin modifier
#define T 99 //Max Texture resolution (assumes square textures)
#define UPSPEED .7 //Max vertical speed ball will move at from speedgen()
#define INCREASE .03 //Speed increase of the ball with each hit
#define CHARSPEED .6 //Player's movespeed
#define STARTSPEED .4 //Ball's starting LR speed
//Time between frames, 16.67ms is 60fps
#define FRAMETIME 16.67


//This class holds a simple 3D game of 2D pong for JustEngine
class Pong {
public:
	bool forcequit;
	bool sightseeing;

	Pong(bool sightseeing = 0);

	~Pong();

	bool play();

	void SDLReset();

	void Reset();

	//Returns 1 if P2 won, 0 if P1 won
	bool Winner() const;
private:

	//Holds surface to draw on for SDL2
	SDL_Window *window;
	SDL_Surface *target;

	Match game; //Holds all models used by Pong
	const Uint8 *keyboard; //Takes in keyboard input to be read later
	float balldir[3]; //Holds ball velocity vector
	bool status[3]; //Holds the ongoing status of the game FORMAT: [Game Ongoing, P1 Win, P2 Win]

	//Decides winner
	void gameCheck();

	void playerMove(int player, float x, float y, float z);

	/* Handles all user input as a well as returning 0 in the the case the player has pressed Escape
		Moves the players and camera */
	bool keyboardHandler();
};

//Basic collision detection between the ball and various objects
void p1Hit(Match& game, float balldir[3]);
void p2Hit(Match& game, float balldir[3]);
void wallHit(Match& game, float balldir[3]);

//Fun stuff, spins ball according to velocity
void ballSpin(Match& game, float balldir[3]);
//Generates the new speed for the ball based on rand()
float speedGen();
#endif