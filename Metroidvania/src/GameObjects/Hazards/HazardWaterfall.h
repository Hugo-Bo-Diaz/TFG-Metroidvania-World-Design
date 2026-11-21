#ifndef HAZARD__WATERFALL
#define HAZARD__WATERFALL

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "Modules/Render.h"

class HazardWaterfall : public GameObject
{
public:
	HazardWaterfall();
	HazardWaterfall(std::list<ObjectProperty*>& aList) { new (this) HazardWaterfall; };

	void Init();
	bool Loop(float dt);
	bool Render();

	Animation anim;
	Timer water_animation;
	float iteration_time = 100;
	RexTextureID hazards;

	RexTextureID particles;
	particle_preset water_drip;
	RXRect r18waterfirst;
	RXRect r19watersecond;
	RXRect r20waterthird;
	ParticleEmitter* mEmitter;
};

#endif