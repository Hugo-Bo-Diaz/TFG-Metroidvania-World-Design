#ifndef PHYSICS__H
#define PHYSICS__H

#include "PartsDef.h"
#include "Textures.h"
#include "Application.h"
#include "Render.h"

#include <list>
#include <vector>

#include "SDL/include/SDL.h"
#include <functional>

#define MAX_WALLS 500

struct SDL_Texture;


enum texture_type
{
	PLAYER_TEX,
	NUMBER_OF_TEXTURES
};

class GameObject
{
public:
	//physics

	SDL_Rect* collider;
	SDL_Rect* nextpos;

	objectId type;

	virtual bool Loop(float dt) { return true; };
	virtual bool Render() { return true; };

	GameObject()
	{
		collider = new SDL_Rect({ 0,0,0,0 });
		nextpos = new SDL_Rect({ 0,0,0,0 });
	}
	~GameObject()
	{
	}
	//visuals
	virtual void Destroy()
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
	objectId type;
	GameObject* object;
};



class Physics : public Part
{
public:

	Physics();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	bool Clearphysics();
	void DeleteObject(GameObject*);
	void GetNearbyWalls(int x, int y, int pxls_range, std::vector<SDL_Rect*>& colliders_near);
	std::vector<GameObject*>* GetAllObjectsOfType(objectId type);

	void GetCollisions(SDL_Rect* rect,std::vector<collision*>&collisions);
	void ClearCollisionArray(std::vector<collision*>&collisions);

	int AddWall(SDL_Rect rect);
	void DeleteWall(int id);

	bool is_paused = false;
	void PauseObjects() { is_paused = true; };
	void UnPauseObjects() { is_paused = false; };

	GameObject* AddObject(int x, int y, int w_col, int h_col, objectId type, std::map<std::string, float>* properties = nullptr);
	
	std::function<GameObject* (objectId, std::map<std::string, float>*)> CallBack;
	std::function<objectId(const char*)> typeCallBack;

	SDL_Rect* walls[MAX_WALLS];

private:

	std::list<GameObject*> objects;
	std::list<GameObject*>to_delete;
};

#endif // !PHYSICS__H
