#ifndef PHYSICS__H
#define PHYSICS__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"

#include <list>
#include <vector>

#include "SDL/include/SDL.h"

#define MAX_WALLS 500

struct SDL_Texture;

enum object_type
{
	//PLAYER
	PLAYER,

	//PROJECTILES
	FIRE_BALL,
	WIND_SLASH,
	ICE_SHARD,
	ICE_BLOCK,
	CLOUD,
	LEAF,
	THORNS,
	ROCK,
	SHOCKWAVE,

	//SPELLS UNLOCK
	FIRE_SPELL_PICKUP,
	GROUND_SPELL_PICKUP,

	//ITEMS
	MAX_HEALTH_PICKUP,
	MAX_MANA_PICKUP,

	//ENEMIES
	GROUNDED_ELEMENTAL,
	FLYING_ELEMENTAL,
	COAL_JUMPER,
	ARMOR_TRAP,
	SHIELD_MONSTER,
	CLING_CREATURE,

	//HAZARDS
	LAVA_HAZARDS,
	LAVA_HAZARD_WATERFALL,
	HAZARDS_SPIKES,
	HAZARDS_ROCK_BLOCK,

	//OTHER
	CHECKPOINT,
	FIRST_DIALOGUE,
	TEXTBOXOBJECT,
	ENDDEMOOBJECT,

	//MAX
	MAX_OBJECT_TYPE
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
	virtual bool Render() { return true; };

	physobj()
	{
		collider = new SDL_Rect({ 0,0,0,0 });
		nextpos = new SDL_Rect({ 0,0,0,0 });
	}
	//visuals
	virtual ~physobj()
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

	void GetCollisions(SDL_Rect* rect,std::vector<collision*>&collisions);
	void ClearCollisionArray(std::vector<collision*>&collisions);

	int AddWall(SDL_Rect rect);
	void DeleteWall(int id);

	bool is_paused = false;
	void PauseObjects() { is_paused = true; };
	void UnPauseObjects() { is_paused = false; };

	physobj* AddObject(int x, int y, int w_col, int h_col, object_type type);

	SDL_Rect* walls[MAX_WALLS];

private:

	std::list<physobj*> objects;

	std::list<physobj*>to_delete;


};

#endif // !PHYSICS__H
