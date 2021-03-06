#ifndef WATER__H
#define WATER__H

#include "Spell.h"
#include "Animation.h"
#include "Timer.h"

class Cloud;

class Water : public Spell
{
public:
	void Init();

	void Loop(float dt);

	void UnlockMovementEvent();

	void CleanUp();

	//iceshard

	//iceblock
	bool is_iceblock_on_cooldown = false;
	float cast_time_block = 100;
	float cooldown_block = 10000;
	Timer iceblock_timer;

	//cloud
	bool is_cloud_on_cooldown = false;
	float cast_time_cloud = 300;
	float cooldown_cloud = 6000;
	Timer cloud_timer;
	float max_radius = 320;
	float min_radius = 32;

	Cloud* current_cloud;
};

#endif