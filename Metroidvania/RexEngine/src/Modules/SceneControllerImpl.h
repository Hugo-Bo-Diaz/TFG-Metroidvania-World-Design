#ifndef SCENE_CONTROLLER_IMPL__H
#define SCENE_CONTROLLER_IMPL__H

#include "../include/Modules/SceneController.h"
#include "PartImpl.h"

struct tileset
{
	int firstgid;

	int tile_width;
	int tile_height;

	int columns;
	int total_tiles;

	TextureID texture;
};


struct DLL_EXPORT layer
{
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
	TextureID texture;
	float parallax_x;
	float parallax_y;
	int depth;
	std::string path;

	bool repeat_y;
};


class SceneController::SceneControllerImpl : public Part::Part_Impl
{
public:
	SceneController::SceneControllerImpl(SceneController* aSceneController) :mPartInst(aSceneController) {};

	void SetSceneControllerInst(SceneController* aSceneController)
	{
		mPartInst = aSceneController;
	}

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

private:

	bool LoadBackground(pugi::xml_node&);
	bool LoadWalls(pugi::xml_node&);
	bool LoadObjects(pugi::xml_node&);
	bool LoadTiles(pugi::xml_node&);

	bool LoadTilesets(pugi::xml_node&);
	bool LoadBackgroundImage(pugi::xml_node&);

	bool LoadMapExecute(const char* filename);
	void LoadMapProperties(pugi::xml_node&);

	int room_w;
	int room_h;

	std::string lMapToLoad;

	//SAME ALL MAPS
	std::vector<background_texture*>backgrounds;
	std::vector<tileset*> tilesets;

	//CHANGE ALL MAPS
	std::vector<layer*> layers;
	std::vector<background_texture*> active_backgrounds;

	std::function<void()> SceneFunction;
	std::function<void()> LoadFunction;


	friend class SceneController;
	SceneController* mPartInst;
};

#endif