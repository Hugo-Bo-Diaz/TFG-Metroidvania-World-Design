#include "..\..\include\EngineElements\Animation.h"
#include "..\..\include\EngineElements\Animation.h"
#include "..\..\include\EngineElements\Animation.h"
#include "EngineElements/Animation.h"

void Animation::AddFrame(const RXRect& frame)
{
	frames.push_back(frame);
}

void Animation::NextFrame()
{
	++current_frame;
	if (current_frame >= frames.size()-1)
	{
		current_frame = 0;
	}
}

RXRect& Animation::GetCurrentFrame()
{
	return frames[current_frame];
}

RXRect& Animation::GetFrame(int ind)
{
	return frames[ind];
}

void Animation::SetCurrentFrame(int aNewCurrentFrame)
{
	current_frame = aNewCurrentFrame;
}

int Animation::GetAmountOfFrames()
{
	return current_frame;
}

int Animation::GetCurrentFrameNumber()
{
	return current_frame;
}
