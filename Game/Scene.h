#ifndef SCENE__H
#define SCENE__H

struct SDL_Texture;
#include "SDL/include/SDL.h"

class Scene
{
public:
	
	virtual bool Create() { return true; };
	virtual bool Start() { return true; };
	virtual bool Loop() { return true; };
	virtual bool Stop() { return true; };
	virtual bool Destroy() { return true; };

	void activate() { active = true; Start(); };
	void deactivate() { active = false; Stop(); };
	SDL_Texture* background;

private:
	

	bool active = false;
	//UI_ITEMS_LIST std::list<ui_stuff> lol;
};

#endif