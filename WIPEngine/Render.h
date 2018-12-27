#ifndef __Render__h__
#define __Render__h__
#include <SDL.h>
#include "SDLHelper.h"
#include "Game.h"
#define HEIGHT 1080
#define WIDTH 1920

IvMatrix44 ndc(float width, float height);
void playerRender(SDL_Surface *target, Match play, IvMatrix44 screen);
void sceneRender(SDL_Surface *target, Match play, IvMatrix44 screen);
bool onScreen(IvMatrix44 mod2screen, std::vector<Vector4> bounds);



void renderUpdate(SDL_Surface *target, Match play) {
	//First we clear the screen of the previously drawn frame
	SDL_FillRect(target, NULL, 0x000000);
	//Then we lock the surface so it's safe to access directly
	SDL_LockSurface(target);	
	IvMatrix44 ndc2screen = ndc(WIDTH, HEIGHT);
	playerRender(target, play, ndc2screen);
	sceneRender(target, play, ndc2screen);
	SDL_UnlockSurface(target);
}

void playerRender(SDL_Surface *target, Match play, IvMatrix44 screen) {
	Vector4 toPrint;
	IvMatrix44 mod2screen = screen * play.POV.perspective * play.POV.world2view * play.player.mod2world;
	if (onScreen(mod2screen, play.player.bounds)) {
		for (int i = 0; i < play.player.pointcount; i++) {
			toPrint = mod2screen * play.player.points[i];
			toPrint.normalize();
			if (toPrint.mV[2] < 1 & toPrint.mV[2] > 0) {
				setBlock(target, toPrint.mV[0], toPrint.mV[1], SDL_MapRGB(target->format, 0xff, 0xff, 0xff));
			}
		}
	}
}

void sceneRender(SDL_Surface *target, Match play, IvMatrix44 screen) {
	Vector4 toPrint;
	IvMatrix44 mod2screen;
	for (int i = 0; i < play.sceneCount; i++) {
		mod2screen = screen * play.POV.perspective * play.POV.world2view * play.scene[i].mod2world;

		if (onScreen(mod2screen, play.scene[i].bounds)) {
			for (int j = 0; j < play.scene[i].pointcount; j++) {
				toPrint = mod2screen * play.scene[i].points[j];
				toPrint.normalize();
				if (toPrint.mV[2] < 1 & toPrint.mV[2] > 0) {
					setBlock(target, toPrint.mV[0], toPrint.mV[1], SDL_MapRGB(target->format, 0xff, 0xff, 0xff));
				}
			}
		}
	}
}

IvMatrix44 ndc(float width, float height) {
	IvMatrix44 toRet;
	toRet.mV[0][0] = width / 2;
	toRet.mV[0][1] = 0;
	toRet.mV[0][2] = 0;
	toRet.mV[0][3] = width / 2 + 0;
	toRet.mV[1][0] = 0;
	toRet.mV[1][1] = -1 * height / 2;
	toRet.mV[1][2] = 0;
	toRet.mV[1][3] = height / 2 + 0;
	toRet.mV[2][0] = 0;
	toRet.mV[2][1] = 0;
	toRet.mV[2][2] = 1.0 / 2.0;
	toRet.mV[2][3] = 1.0 / 2.0;
	toRet.mV[3][0] = 0;
	toRet.mV[3][1] = 0;
	toRet.mV[3][2] = 0;
	toRet.mV[3][3] = 1.0;

	return toRet;
}

/*Performs basic culling using the bounding box by checking to see if any of the bounding box would be rendered
	Note:	for items of less than 8 vertices this is a drop in performance, but overall there will never really be times where
			a model has only 8 vertices
*/
bool onScreen(IvMatrix44 mod2screen, std::vector<Vector4> bounds) {
	Vector4 test;
	for (int i = 0; i < bounds.size(); i++) {
		test = mod2screen * bounds[i];
		test.normalize();
		//Only accepts a point if it fits in the view frame and 
		if (test.mV[2] <= 1 & test.mV[2] >= 0 & test.mV[0] >= 0 & test.mV[0] <= WIDTH & test.mV[1] >= 0 & test.mV[1] <= HEIGHT) {
			return 1;
		}
	}
	return 0;
}
#endif