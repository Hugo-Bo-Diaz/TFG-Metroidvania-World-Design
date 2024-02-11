#ifndef METROIDVANIA_SCENEPROCESSOR
#define METROIDVANIA_SCENEPROCESSOR

#include <list>
#include "UIElements/UISettingsMenu.h"
#include "UIElements/UIingameUI.h"

#include "Portal.h"

struct room
{
	int id;
	std::string path;
};

struct LogEntry
{
	std::string title;
	std::string text;
};


class MetroidVaniaSceneProcessor
{
private:

	MetroidVaniaSceneProcessor() {};
public:
	bool canopenmenu = true;
	bool is_pause_menu_up = false;
	bool is_select_menu_up = false;
	bool is_settings_up = false;

	bool should_add_settings = false;
	bool should_start_game = false;
	bool start_load_game = false;
	bool should_exit = false;
	bool should_go_to_main_menu = false;

	UISettingsMenu* settings = nullptr;
	UIingameui* inGameUI = nullptr;
	Player* pl = nullptr;

	int current_room_id = 0;
	int spawn_point_id = 0;

	static MetroidVaniaSceneProcessor& GetInstance();

	void SceneProcessingMainMenu();
	void SceneProcessingInGame();

	void SceneCreationMainMenu();
	void SceneCreationInGame();

	void LoadMapArray(const char* document);
	std::vector<room*> rooms;
	void GoToLastCheckPoint();
	void UsePortal(Portal* p, int offset);
	void GoToLoadedScene();
	std::vector<GameObject*>spawnpoints;

	std::vector<LogEntry*> logs;
	LogEntry* GetLog(int id) {
		if (id < logs.size())
		{
			return logs[id];
		}
		else
		{
			return nullptr;
		}
	}

	std::vector<GameObject*> portals;
};
#endif // !METROIDVANIA_SCENEPROCESSOR