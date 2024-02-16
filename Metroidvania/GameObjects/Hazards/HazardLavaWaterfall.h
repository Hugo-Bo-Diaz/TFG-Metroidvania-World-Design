#ifndef HAZARD__LAVA__WATERFALL
#define HAZARD__LAVA__WATERFALL

#include "Physics.h"
#include "Animation.h"
#include "Textures.h"

class HazardLavaWaterfall : public GameObject
{
public:
	HazardLavaWaterfall();
	bool Loop(float dt);
	bool Render();

	Animation anim;
	Timer lava_animation;
	float iteration_time = 500;
	TextureID hazards;
};

#endif