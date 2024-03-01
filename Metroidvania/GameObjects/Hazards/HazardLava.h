#ifndef HAZARD__LAVA
#define HAZARD__LAVA

#include "Physics.h"
#include "Timer.h"
#include "Textures.h"

class HazardLava :
	public GameObject
{
public:
	HazardLava();
	HazardLava(std::list<ObjectProperty*>& aList) { new (this) HazardLava; };

	bool Loop(float dt);
	bool Render();

	SDL_Rect lava_top1;
	SDL_Rect lava_top2;
	Timer lava_animation;
	float iteration_time=1000;
	bool offset_cycle=true;

	SDL_Rect lava;
	TextureID hazards;
};

#endif