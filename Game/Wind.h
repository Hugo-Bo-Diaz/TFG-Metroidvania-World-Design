#ifndef WIND__H
#define WIND__H

#include "Spell.h"
#include "Animation.h"
#include "Timer.h"

class Wind : public Spell
{
public:
	void Init();

	void Loop(float dt);

	void UnlockMovementEvent();

	void CleanUp();

	//windslash

	//superjump
	bool jump_available = true;
	float jump_force = 22;

	//mobility
	bool is_mobility_on_cooldown = false;
	bool is_mobility_up = false;
	float mobility_activation_time = 100;
	float mobility_max_time = 5000;
	float cooldown_mobility = 10000;
	Timer mobility_timer;

	//sprites
	SDL_Rect fireball_big = { 0,0,48,48 };
	SDL_Rect fireball_small = { 48,0,48,48 };

	Animation firebreath_right;
	Animation firebreath_left;

	Animation fireshield;
};

#endif