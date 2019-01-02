#ifndef __SDLHelper__h__
#define __SDLHelper__h__
#include <SDL.h>

bool renderable(SDL_Surface *target, const int& x, const int& y) {
	if (x >= 1920 || y >= 1080 || x < 0 || y < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void setPixel(SDL_Surface *target, int x, int y, Uint32 pixel) {
	if (x >= 1920 || y >= 1080 || x < 0 || y < 0) {
		return;
	}
	Uint32 *toChange = (Uint32 *)target->pixels;
	toChange[(y * target->w) + x] = pixel;
}

void setBlock(SDL_Surface *target, int x, int y, Uint32 pixel) {
	if (!renderable(target, x, y)) {
		return;
	}
	int width = target->w;
	Uint32 *toChange = (Uint32 *)target->pixels;
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (renderable(target, x + j, y + i)) {
				toChange[((y + i) * width) + (x + j)] = pixel;
			}
		}
	}
}



#endif