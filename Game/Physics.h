#ifndef PHYSICS__H
#define PHYSICS__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"

#include <list>
#include <vector>

#include "SDL/include/SDL.h"

#define MAX_WALLS 30

struct SDL_Texture;

enum object_type
{
	PLAYER,
	FIRE_BALL,
	WIND_SLASH,
	ICE_SHARD,
	ICE_BLOCK,
	CLOUD,
	LEAF,
	THORNS,
	ROCK,
	SHOCKWAVE
};

enum texture_type
{
	PLAYER_TEX,
	NUMBER_OF_TEXTURES
};

class physobj
{
public:
	//physics

	SDL_Rect* collider;
	SDL_Rect* nextpos;

	object_type type;

	virtual bool Loop(float dt) { return true; };

	physobj()
	{
		collider = new SDL_Rect({ 0,0,0,0 });
		nextpos = new SDL_Rect({ 0,0,0,0 });
	}
	//visuals
	~physobj()
	{
		if (collider != nullptr)
		{
			delete collider;
			collider = nullptr;
		}
		if (nextpos != nullptr)
		{
			delete nextpos;
			nextpos = nullptr;
		}
	}
};

struct collision
{
	object_type type;
	physobj* object;
};

class Portal
{
public:
	SDL_Rect area;
	int id_destination;
	bool horizontal;

	void GoToDestination(int diff);
};

class Physics : public Part
{
public:

	Physics();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	bool Clearphysics();
	void DeleteObject(physobj*);
	void GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near);
	
	void GetCollisions(physobj* obj,std::vector<collision*>&collisions);

	int AddWall(SDL_Rect rect);
	void DeleteWall(int id);

	void AddPortal(SDL_Rect area, int destination, bool horizontal = true);
	void DeletePortals();

	physobj* AddObject(int x, int y, int w_col, int h_col, object_type type);

private:

	SDL_Rect* walls [MAX_WALLS];
	std::list<physobj*> objects;

	std::list<physobj*>to_delete;

	std::list<Portal*> portals;

};

#endif // !PHYSICS__H
