#include "IvMatrix33.h"
#include <iostream>
#include <SDL.h>
#undef main

using namespace std;

int main() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!";
	}

	cin.get();
	return 0;
}