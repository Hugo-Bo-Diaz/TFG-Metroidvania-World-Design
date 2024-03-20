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

/* This class contains rectangles and textures, it facilitates rendering of a sequence of sprites
its main limiation is having only one texture per animation*/
class DLL_EXPORT Animation
{
private:
	std::vector<RXRect> frames;
	int current_frame = 0;

public:

	Animation() {};

	//this constructor allows for the direct assignation of a texture
	Animation(TextureID aTex) :mTexture(aTex) {};
	
	//add frames of animation using this function
	void AddFrame(const RXRect& frame);

	//advances to next frame, if this is not called only one frame will show
	void NextFrame();

	//returns the current frame's rectangle inside of the texture
	RXRect& GetCurrentFrame();

	//returns the indicated frame's rectangle inside of the texture
	RXRect& GetFrame(int ind);

	//sets the frame to the indicated index
	void SetCurrentFrame(int aNewCurrentFrame);
	
	//returns the amount of frames an animation has
	int GetAmountOfFrames();
	
	//returns the current frame that is in display now
	int GetCurrentFrameNumber();

	//the texture that will be used on the renderer
	TextureID mTexture = -1;
};

#endif