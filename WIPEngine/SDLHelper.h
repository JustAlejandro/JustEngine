#ifndef __SDLHelper__h__
#define __SDLHelper__h__
#include <SDL.h>

void setPixel(SDL_Surface *target, int x, int y, Uint32 pixel) {
	if (x >= 1920 || y >= 1080 || x <= 0 || y <= 0) {
		return;
	}
	Uint32 *toChange = (Uint32 *)target->pixels;
	toChange[(y * target->w) + x] = pixel;
}

void setBlock(SDL_Surface *target, int x, int y, Uint32 pixel) {
	setPixel(target, x, y, pixel);
	setPixel(target, x + 1, y, pixel);
	setPixel(target, x - 1, y, pixel);
	setPixel(target, x, y + 1, pixel);
	setPixel(target, x + 1, y + 1, pixel);
	setPixel(target, x - 1, y + 1, pixel);
	setPixel(target, x, y - 1, pixel);
	setPixel(target, x + 1, y - 1, pixel);
	setPixel(target, x - 1, y - 1, pixel);

}



#endif