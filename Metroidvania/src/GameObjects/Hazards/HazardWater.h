#ifndef HAZARD__WATER
#define HAZARD__WATER

#include "Modules/SceneController.h"
#include "Utils/Timer.h"
#include "Modules/Render.h"

class HazardWater :
	public GameObject
{
public:
	HazardWater();
	HazardWater(std::list<ObjectProperty*>& aList) { new (this) HazardWater; };

	void Init();
	bool Loop(float dt);
	bool Render();

	RXRect water_top1;
	RXRect water_top2;
	RXRect water_top3;
	RXRect water_top4;
	Timer water_animation;

	float iteration_time=250;
	int offset_cycle=0;

	RXRect water;
	RexTextureID hazards;
};

#endif