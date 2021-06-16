#ifndef SCENECONTROLLER__H
#define SCENECONTROLLER__H

#include "PartsDef.h"
#include<vector>
#include "Text.h"
#include "SDL/include/SDL.h"

#define ROOMS_MAX_X 10
#define ROOMS_MAX_Y 10

class Portal;

enum layer_type
{
	BACKGROUND,
	TILES,
	OBJECT,
	UI
};


struct tileset
{
	int firstgid;

	int tile_width;
	int tile_height;

	int columns;
	int total_tiles;

	SDL_Texture* texture;
};


struct layer
{
	layer_type type;
	uint* data;
	int width;
	int height;
	float parallax_x;
	float parallax_y;
	int depth;
	int size;

	tileset* tileset_of_layer;

	~layer()
	{
		delete data;
	}
};

struct background_texture
{
	SDL_Texture* texture;
	float parallax_x;
	float parallax_y;
	int depth;
	std::string path;

	bool repeat_y;
};

struct room
{
	int id;
	std::string path;
};

struct SpawnPoint
{
	int x;
	int y;
	int id;
};

class Portal
{
public:
	SDL_Rect area;
	int id_destination_room;
	int id_destination_point;
	bool horizontal;
};

class SceneController : public Part
{
public:
	SceneController();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadAssetsMap(const char* map);
	bool LoadTilesets(pugi::xml_node&);
	bool LoadBackgroundImage(pugi::xml_node&);
	void LoadMapArray(const char* document);


	bool LoadMap(const char* filename);
	void LoadMapProperties(pugi::xml_node&);
	void CleanMap();
	void ChangeMap(const char* filename);

	bool LoadBackground(pugi::xml_node&);
	bool LoadWalls(pugi::xml_node&);
	bool LoadObjects(pugi::xml_node&);
	bool LoadTiles(pugi::xml_node&);
	bool LoadPortals(pugi::xml_node&);
	bool LoadSpawnPoints(pugi::xml_node&);
	bool LoadCheckPoints(pugi::xml_node&);

	tileset* GetTilesetFromId(int id);
	SDL_Rect GetImageRectFromId(tileset* t, int id);
	SDL_Texture* GetTextureFromId(int id);

	int RoundToNearestTileset(int id);

	void RenderTiles();
	void RenderBackground();

	uint song_try;

	int room_w;
	int room_h;

	void UsePortal(Portal*p, int offset);
	std::vector<SpawnPoint*>spawnpoints;
	void AddPortal(SDL_Rect area, int destination_r, int destination_p, bool horizontal = true);
	void AddPortal(Portal* p);
	void DeletePortals();
	void GoToLastCheckPoint();
	void GoToLoadedScene();
	void GoToMainMenu();

	int spawnpoint_x;
	int spawnpoint_y;

	int current_room_id = 0;

	bool is_pause_menu_up = false;
	
	bool is_select_menu_up = false;

private:

	//SAME ALL MAPS
	std::vector<background_texture*>backgrounds;
	std::vector<tileset*> tilesets;
	
	//CHANGE ALL MAPS
	std::vector<layer*> layers;
	std::vector<background_texture*> active_backgrounds;

	std::vector<room*> rooms;

	std::list<Portal*> portals;

};

#endif // !SCENECONTROLLER__H

