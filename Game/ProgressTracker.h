#ifndef PROGRESS__TRACKER__H
#define PROGRESS__TRACKER__H

#include "PartsDef.h"
#include<vector>

#include "SDL/include/SDL.h"

#define MAX_EXPANSIONS 32

struct LoreLog
{
	std::string title;
	std::string text;
	int id;

	bool operator<(const LoreLog& rhs) const
	{
		if (id < rhs.id)
			return true;
		else
			return false;
	}
};

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
	bool CanLoadGame(const char* file);

	void SetPlayer(Player* p);
	int spawn_point = 0;
	void AddPlayer();
	void RespawnPlayer();

	void SetPlayerToLastCheckPoint();
	int last_checkpoint_x = 0;
	int last_checkpoint_y = 0;
	int last_checkpoint_id = 0;
	void SetCheckPoint(int x, int y, int room_id);
	bool send_player_to_checkpoint = false;


	bool start_new_game = false;
	void StartNewGame();
	bool start_load_game = false;
	void StartLoadGame();
	bool send_player_to_load = false;

	bool go_to_main_menu = false;
	bool should_exit = false;

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

	Player* pl = nullptr;

	bool itemspickedup[MAX_EXPANSIONS];
	void AddPickupToList(int id);
	bool HasBeenFound(int id);

	bool respawn_player = false;

	//lore logs handling
	std::vector<LoreLog*> lore_logs;
	std::vector<LoreLog*> active_logs;
	void LoadLogs(const char* logs_file);
	void UnlockLog(int id);
	LoreLog* GetLog(int number);
	void OrderActiveLogs();
	bool HasLogBeenUnlocked(int id);

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

};

#endif // !CAMERA__H
