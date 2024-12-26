#ifndef HAZARD__LAVA__WATERFALL
#define HAZARD__LAVA__WATERFALL

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "Modules/Render.h"

class HazardLavaWaterfall : public GameObject
{
public:
	HazardLavaWaterfall();
	HazardLavaWaterfall(std::list<ObjectProperty*>& aList) { new (this) HazardLavaWaterfall; };

	void Init();
	bool Loop(float dt);
	bool Render();

	Animation anim;
	Timer lava_animation;
	float iteration_time = 500;
	RexTextureID hazards;
};

#endif