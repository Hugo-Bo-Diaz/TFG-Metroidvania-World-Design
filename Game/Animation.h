#ifndef ANIMATION__H
#define ANIMATION__H

#include <vector>
#include "SDL/include/SDL.h"

#define MAXFRAMES 20

enum Animation_loop
{
	LOOP,
	STOP,

};

//an animation is going to be a series of rectangles that encapsulate the sprites
class Animation
{
public:
	Animation();
	
	int current_frame = 0;
	int amount_of_frames=0;
	
	void AddFrame(SDL_Rect frame);

	void NextFrame();

	SDL_Rect* GetCurrentFrame();
	
	SDL_Rect* GetFrame(int ind);

	SDL_Rect frames[MAXFRAMES];

	//implement a callback when the animation is done
};

#endif