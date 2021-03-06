#include "Animation.h"


Animation::Animation()
{
	SDL_Rect zero = { 0,0,0,0 };
	for (int i = 0; i < MAXFRAMES; ++i)
	{
		frames[i] = zero;
	}
}

void Animation::AddFrame(SDL_Rect frame)
{
	frames[amount_of_frames] = frame;
	++amount_of_frames;
}

void Animation::NextFrame()
{
	++current_frame;
	if (current_frame == amount_of_frames)
	{
		current_frame = 0;
	}
}

SDL_Rect* Animation::GetCurrentFrame()
{
	return &frames[current_frame];
}

SDL_Rect* Animation::GetFrame(int ind)
{
	return &frames[ind];
}
