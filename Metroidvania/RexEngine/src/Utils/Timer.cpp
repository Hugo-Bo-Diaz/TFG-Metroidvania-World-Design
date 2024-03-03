#include "Utils/Timer.h"
#include "SDL\include\SDL_timer.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	started_at = SDL_GetTicks();
	paused = false;
	time_paused = 0;
}

float Timer::Read()
{
	if (!paused)
		return SDL_GetTicks() - started_at - time_paused;
	else
		return paused_at - started_at - time_paused;
}

float Timer::Read_Sec()
{
	if (!paused)
		return float(SDL_GetTicks() - started_at - time_paused)/1000.0f;
	else
		return float(paused_at - started_at - time_paused)/1000.0f;
}

void Timer::Pause()
{
	if (!paused)
	{
		paused_at = SDL_GetTicks();
		paused = true;
	}
}

void Timer::Resume()
{
	if (paused)
	{
		resumed_at = SDL_GetTicks();
		time_paused += resumed_at - paused_at;
		paused = false;
	}
}

void Timer::Reset()
{
	started_at = SDL_GetTicks();
}