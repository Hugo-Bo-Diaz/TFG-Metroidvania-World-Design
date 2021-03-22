#ifndef PROGRESS__TRACKER__H
#define PROGRESS__TRACKER__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"

class Player;

class ProgressTracker : public Part
{
public:
	ProgressTracker();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void SaveGame(const char* file);
	void LoadGame(const char* file);

	void SetPlayer(Player* p);

	//variables that track events in the game

	bool is_water_enabled;
	bool is_fire_enabled;
	bool is_ground_enabled;
	bool is_wind_enabled;
	bool is_grass_enabled;
	
};

#endif // !CAMERA__H
