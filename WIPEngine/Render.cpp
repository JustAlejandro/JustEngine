#include "Render.h"
#include "SDLHelper.h"
#include "Textures.h"
//MipMap base sample count
#define MAP 250

//Global ndc2screen giving the option to change it later but also meaning that we don't need to change it or declare it
IvMatrix44 ndc2screen = ndc(WIDTH, HEIGHT);

void renderUpdate(SDL_Surface *target, float depth[WIDTH*HEIGHT / 4], Match play) {
	//First we clear the screen of the previously drawn frame
	SDL_FillRect(target, NULL, 0x000000);
	oneArray(depth, WIDTH / 2, HEIGHT / 2);
	//Then we lock the surface so it's safe to access directly
	SDL_LockSurface(target);
	sceneRender(target, depth, play, ndc2screen);
	SDL_UnlockSurface(target);
}

//Handle culling as well as draw calls to setTriangle
void sceneRender(SDL_Surface *target, float depth[WIDTH*HEIGHT / 4], const Match& play, const IvMatrix44& screen) {
	//We store the previous 2 points in order to generate triangles
	Vector4 toPrint;
	Vector4 previous;
	Vector4 previous2;
	IvMatrix44 mod2screen;
	for (int i = 0; i < play.sceneCount; i++) {
		mod2screen = screen * play.POV.perspective * play.POV.world2view * play.scene[i].mod2world;

		//Runs through each member of the scene and tests if it is on screen
		if (onScreen(mod2screen, play.scene[i].bounds)) {
			//Calls setTriangle if the object is on screen
			for (int j = 0; j < play.scene[i].pointcount; j++) {
				toPrint = mod2screen * play.scene[i].points[j];
				toPrint.homDiv();
				//Don't have enough parameters the first two runs, since we're using triangle strip coordinates
				if (j > 1) {
					setTriangle(target, depth, play.scene[i].texture, toPrint, previous, previous2, play.scene[i].textureCoord[j], play.scene[i].textureCoord[j - 1], play.scene[i].textureCoord[j - 2]);
				}
				previous2 = previous;
				previous = toPrint;
			}
		}
	}
}

//Fills a triangle that is defined as bound with point one being the origin and points two and three defining vectors relative to point one
//Problems: triangle fill doesn't take into account the field of view and fills triangles uniformly, resulting in skewing when viewed off center, especially when the rendered points are far off screen where the FOV skews heavily
void setTriangle(SDL_Surface *target, float depth[WIDTH*HEIGHT / 4], int textureIndex, Vector4 one, Vector4 two, Vector4 three, const std::pair<int, int>& tex1, const std::pair<int, int>& tex2, const std::pair<int, int>& tex3) {
	/*Checks that all points are within the view frame in the z Direction
			Problems: results in triangles not being rendered if one of the points maps out too far in the -z axis
						Could just render the triangle's anyway, but we end up with very odd results due to the z mappings being incorrect for points behind the camera
						Best option is to implement clipping and let it take care of the issue
	*/
	
	if ((one.mV[2] < 1 && one.mV[2] > -1) && (two.mV[2] < 1 && two.mV[2] > -1) && (three.mV[2] < 1 && three.mV[2] > -1)) {
		//Here we redefine two and three to be vectors that show direction relative to one
		two = (two - one);
		three = (three - one);

		//Using this we're able to do a rudimentary form of mip mapping, where the larger a triangle is rendered, the more samples we take from its original texture
		float samplesize = 10.0;
		//A basic estimate of the amount of ground needing to be covered by the fill, better thing would be to find the area of the triangle, but that would take time
		float sizeEst = abs(two.mV[0]) + abs(three.mV[0]) + abs(two.mV[1]) + abs(three.mV[1]);
		if (sizeEst > 100) {
			samplesize = MAP * .75;
			if (sizeEst > 300) {
				samplesize = MAP * .85;
				if (sizeEst > 500) {
					samplesize = MAP;
					if (sizeEst > 750) {
						samplesize = MAP * 1.4;
						if (sizeEst > 1000) {
							samplesize = MAP * 1.5;
							if (sizeEst > 2400) {
								samplesize = MAP * 1.7;
							}
						}
					}
				}
			}
		}
		
		//Converting two and three to slopes so we can map out the triangle, something similar to this will possibly be used by clipping when it's implemented
		two = two / samplesize;
		three = three / samplesize;
		//Fill holds a basic vector running from point two to three, this will be used to generate the triangle between our initial points of one, two, and three.
		Vector4 fill = (three - two);
		Vector4 twoCopy = two;

		//These values are unnecessary but make debugging easier with a fairly small performance impact so they'll stay for now
		float x = 0;
		float y = 0;
		float z = 0;

		//Same with the other triangle fill vectors, except this one holds the slopes for texture filling
		float texX[2] = { (tex2.first - tex1.first) / samplesize, (tex2.second - tex1.second) / samplesize };
		float texY[2] = { (tex3.first - tex1.first) / samplesize - texX[0], (tex3.second - tex1.second) / samplesize - texX[1] };
		//Used for debugging to find the final texture coordinate
		int texCoord;
		//For a basic visual representation of what this does https://i.imgur.com/MiRVy22.gifv
		for (int i = 0; i < (int) samplesize; i++) {
			//We only need to update this once per loop so we'll set it up outside the inner loop
			twoCopy = two * i;

			for (int j = 0; j < i; j++) {
				//Check z-depth before doing extra math
				z = one.mV[2] + twoCopy.mV[2] + fill.mV[2] * j;
				if (z < 1 && z > -1) {
					x = one.mV[0] + twoCopy.mV[0] + fill.mV[0] * j;
					y = one.mV[1] + twoCopy.mV[1] + fill.mV[1] * j;

					texCoord = (tex1.first + (int)(texX[0] * i + texY[0] * j)) * 300 + (tex1.second + (int)(texX[1] * i + texY[1] * j)) * 3;

					if (inFrame(x, y) && z < depth[(int)x / 2 + (int)y / 2 * WIDTH / 2]) {
						//setBlock allows for us to render to a high resolution window with a low resolution frame. In the future may be able to take the time between frames and an adaptive implementation of setBlock with a changing MAP constant for adaptive resolution
						setBlock(target, x, y, SDL_MapRGB(target->format, texLibrary[textureIndex].pixel_data[texCoord], texLibrary[textureIndex].pixel_data[texCoord + 1], texLibrary[textureIndex].pixel_data[texCoord + 2]));
						//Our depth map is set to 1/4 resolution of the final output because otherwise we would have to sample far too many points and it would slow down our rendering
						//	Also: since we don't set out pixels one at a time this would result in some random 'noise' if even a single point failed the depth test. To see this flaw in action set MAP to 50
						depth[(int)x / 2 + (int)y / 2 * WIDTH / 2] = z;
					}
				}
			}

		}
	}
}


//Clears out the array that holds depth in order to prepare to draw the next frame
void oneArray(float depth[WIDTH * HEIGHT / 4], int width, int height) {
	for (int i = 0; i < width*height; i++) {
		depth[i] = 1;
	}
}

/*NDC to screen coordinates function
	could make this a constant value, but then we can't change resolution in the middle of the program.
*/
IvMatrix44 ndc(float width, float height) {
	IvMatrix44 toRet;
	toRet.mV[0][0] = width / 2;
	toRet.mV[0][1] = 0;
	toRet.mV[0][2] = 0;
	//Lines like this with + 0 are there to show that we can add a height and width offset so that we could do a splitscreen render with two different Cameras
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
		test.homDiv();
		//Only accepts a point if it fits in the view frame and 
		if (test.mV[2] <= 1 & test.mV[2] >= -1 & test.mV[0] >= 0 & test.mV[0] <= WIDTH & test.mV[1] >= 0 & test.mV[1] <= HEIGHT) {
			return 1;
		}
	}
	return 0;
}

//Simply checks to see if we're trying to draw outside of the view coordinates
bool inFrame(int x, int y) {
	if (x >= WIDTH || x < 0) {
		return 0;
	}
	if (y >= HEIGHT || y < 0) {
		return 0;
	}
	return 1;
}