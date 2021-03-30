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
	int spawn_point = 0;
	void AddPlayer();
	void RespawnPlayer();

	bool start_new_game = false;
	void StartNewGame();
	//variables that track events in the game

	std::vector<bool> unlocked;
	
	Player* pl;

};

#endif // !CAMERA__H
