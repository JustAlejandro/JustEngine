#include <iostream>
#include <SDL.h>
#include "Render.h"
#include "Game.h"
#undef main

using namespace std;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Just Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
	SDL_Surface *target = SDL_GetWindowSurface(window);

	SDL_FillRect(target, NULL, SDL_MapRGB(target->format, 0x00, 0xFF, 0xFF));
	float verts[8][4] = { { -.5,-.5,-.5,1 }, { -.5,-.5,.5,1 }, { -.5,.5,-.5,1 }, { -.5,.5,.5,1 }, { .5,-.5,-.5,1 }, { .5,-.5,.5,1 }, { .5,.5,-.5,1 }, { .5,.5,.5,1 } };
	float rot[3] = { 0,0,0 };
	float pos[3] = { 6,0,0 };
	float sca[3] = { 3,3,3 };
	Model test(verts, 8, pos, rot, sca);
	Model test2(test);

	bool run = 1;
	int frame = 0;
	SDL_Event board;
	Match play(test, &test2, 1);
	while (run == 1) {
		while (SDL_PollEvent(&board) != 0) {
			switch (board.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				run = 0;
				break;
			case SDLK_w:
				play.player.move(0, 0, .05);
				break;
			case SDLK_a:
				play.player.move(0, .05, 0);
				break;
			case SDLK_s:
				play.player.move(0, 0, -.05);
				break;
			case SDLK_d:
				play.player.move(0, -.05, 0);
				break;
			case SDLK_UP:
				play.player.move(.1, 0, 0);
				break;
			case SDLK_DOWN:
				play.player.move(-.1, 0, 0);
				break;
			case SDLK_LEFT:
				play.player.spin(0, 0, 2);
				break;
			case SDLK_RIGHT:
				play.player.spin(0, 0, -2);
			default:
				break;
			}
		}
		renderUpdate(target, play);
		SDL_UpdateWindowSurface(window);
		frame++;
		cout << frame << endl;
		SDL_Delay(16);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
