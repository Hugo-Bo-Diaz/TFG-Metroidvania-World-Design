#ifndef HAZARD__LAVA__WATERFALL
#define HAZARD__LAVA__WATERFALL

#include "Physics.h"
#include "Animation.h"

class HazardLavaWaterfall : public physobj
{
public:
	HazardLavaWaterfall();
	bool Loop(float dt);
	bool Render();

	Animation anim;
	Timer lava_animation;
	float iteration_time = 500;

};

#endif