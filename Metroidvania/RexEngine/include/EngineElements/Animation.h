#ifndef ANIMATION__H
#define ANIMATION__H

#include <vector>
#include "RXRect.h"

#define DLL_EXPORT __declspec(dllexport)

#define MAXFRAMES 20

typedef unsigned int TextureID;

enum Animation_loop
{
	ANIM_LOOP,
	ANIM_STOP,

};

//an animation is going to be a series of rectangles that encapsulate the sprites
class DLL_EXPORT Animation
{
private:
	std::vector<RXRect> frames;
	int current_frame = 0;

public:
	Animation() {};
	Animation(TextureID aTex) :mTexture(aTex) {};
	void AddFrame(const RXRect& frame);

	void NextFrame();
	RXRect& GetCurrentFrame();
	RXRect& GetFrame(int ind);
	void SetCurrentFrame(int aNewCurrentFrame);
	int GetAmountOfFrames();
	int GetCurrentFrameNumber();

	TextureID mTexture = -1;
	//implement a callback when the animation is done
};

#endif