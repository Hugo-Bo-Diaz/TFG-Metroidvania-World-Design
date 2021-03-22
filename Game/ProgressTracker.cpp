#include "ProgressTracker.h"

ProgressTracker::ProgressTracker()
{
	name = "ProgressTracker";
}

bool ProgressTracker::Init()
{
	return true;
}

bool ProgressTracker::Loop(float dt)
{
	return true;
}

bool ProgressTracker::CleanUp()
{
	return true;
}

void ProgressTracker::SaveGame(const char * file)
{
}

void ProgressTracker::LoadGame(const char * file)
{
}

void ProgressTracker::SetPlayer(Player * p)
{
}
