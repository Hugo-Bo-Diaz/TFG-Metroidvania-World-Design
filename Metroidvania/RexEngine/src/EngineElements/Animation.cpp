#include "EngineElements/Animation.h"


Animation::Animation()
{
	RXRect zero = { 0,0,0,0 };
	for (int i = 0; i < MAXFRAMES; ++i)
	{
		frames[i] = zero;
	}
}

void Animation::AddFrame(RXRect frame)
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

RXRect* Animation::GetCurrentFrame()
{
	return &frames[current_frame];
}

RXRect* Animation::GetFrame(int ind)
{
	return &frames[ind];
}
