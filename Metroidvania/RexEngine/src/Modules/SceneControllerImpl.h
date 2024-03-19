#ifndef SCENE_CONTROLLER_IMPL__H
#define SCENE_CONTROLLER_IMPL__H

#include "../include/Modules/SceneController.h"
#include "PartImpl.h"

struct tileset
{
	tileset(int aFirstgid, int aTile_width, int aTile_height, int aColumns, int aTotal_tiles) 
		: firstgid(aFirstgid), tile_width(aTile_width), tile_height(aTile_height), columns(aColumns), total_tiles(aTotal_tiles) {};

	int firstgid;

	int tile_width;
	int tile_height;

	int columns;
	int total_tiles;

	TextureID texture;
};


struct layer
{
	uint* data;
	int width;
	int height;
	float parallax_x;
	float parallax_y;
	int depth;
	int size;

	tileset* tileset_of_layer;

	layer(tileset* aTileset,uint* aData, int aWidth, int aHeight, float aParallax_x, float aParallax_y, int aDepth, int aSize)
		: tileset_of_layer(aTileset), data(aData), width(aWidth), height(aHeight), parallax_x(aParallax_x), parallax_y(aParallax_y), depth(aDepth), size(aSize) {}

	~layer()
	{
		delete data;
	}
};

struct background_texture
{
	background_texture(TextureID aTexID, float aParallax_x, float aParallax_y, int aDepth, const char* aPath, bool aRepeat_y)
		: texture(aTexID), parallax_x(aParallax_x), parallax_y(aParallax_y), depth(aDepth), path(aPath), repeat_y(aRepeat_y) {};

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

	bool LoadTilesets(pugi::xml_node&, const char* aMapFolder);
	bool LoadBackgroundImage(pugi::xml_node&, const char* aMapFolder);

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