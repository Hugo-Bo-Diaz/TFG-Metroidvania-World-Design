#ifndef PROGRESS__TRACKER__H
#define PROGRESS__TRACKER__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"

#define MAX_EXPANSIONS 32

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
	
	float player_hp = 4;
	float max_player_hp = 4;

	int charges_hp;
	int charges_per_hp = 4;

	float player_mana = 3;
	float max_player_mana = 3;

	int charges_mana;
	int charges_per_mana = 4;

	Player* pl;

	bool itemspickedup[MAX_EXPANSIONS];
	void AddPickupToList(int id);
	bool HasBeenFound(int id);

	bool respawn_player = false;

};

#endif // !CAMERA__H
