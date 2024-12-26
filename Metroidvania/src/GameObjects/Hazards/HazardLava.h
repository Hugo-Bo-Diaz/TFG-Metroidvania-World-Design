#ifndef HAZARD__LAVA
#define HAZARD__LAVA

#include "Modules/SceneController.h"
#include "Utils/Timer.h"
#include "Modules/Render.h"

class HazardLava :
	public GameObject
{
public:
	HazardLava();
	HazardLava(std::list<ObjectProperty*>& aList) { new (this) HazardLava; };

	void Init();
	bool Loop(float dt);
	bool Render();

	RXRect lava_top1;
	RXRect lava_top2;
	Timer lava_animation;
	float iteration_time=1000;
	bool offset_cycle=true;

	RXRect lava;
	RexTextureID hazards;
};

#endif