#ifndef __Render__h__
#define __Render__h__
#include <SDL.h>
#include "Game.h"
#define HEIGHT 1080
#define WIDTH 1920

IvMatrix44 ndc(float width, float height);
void renderUpdate(SDL_Surface * target, float depth[WIDTH*HEIGHT / 4], Match play);
void sceneRender(SDL_Surface *target, float depth[WIDTH*HEIGHT / 4], const Match& play, const IvMatrix44& screen);
bool onScreen(IvMatrix44 mod2screen, std::vector<Vector4> bounds);
void setTriangle(SDL_Surface *target, float depth[WIDTH*HEIGHT / 4], int textureIndex, Vector4 one, Vector4 two, Vector4 three, const std::pair<int, int>& tex1, const std::pair<int, int>& tex2, const std::pair<int, int>& tex3);
void oneArray(float depth[WIDTH*HEIGHT / 4], int width = WIDTH / 2, int height = HEIGHT / 2);
bool inFrame(int x, int y);


#endif