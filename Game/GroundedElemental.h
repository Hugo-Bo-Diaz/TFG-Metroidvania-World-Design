#ifndef GROUNDED__ELEMENTAL__H
#define GROUNDED__ELEMENTAL__H

#include "Physics.h"
#include "Animation.h"


enum GroundedElementalColor
{
	RED_GROUNDELEMENTAL
};

class GroundedElemental : public physobj
{
public:
	GroundedElemental();
	~GroundedElemental();

	bool Loop(float dt);
	bool Render();

	float speed_x = 1;
	float speed_y = -10;

	float acceleration_y = 1.0;
	float health = 2;

	float animation_frame_interval=100;
	Timer animation_timer;
	Animation walking_left;
	Animation walking_right;

	void SetAnimations(GroundedElementalColor c);

	GroundedElementalColor c;
};

#endif