#ifndef FIRE__H
#define FIRE__H

#include "Spell.h"
#include "Animation.h"
#include "Timer.h"

class Fire : public Spell
{
public:
	void Init();

	void Loop(float dt);

	void UnlockMovementEvent();

	void CleanUp();

	//fireball
	int charge = 0;

	//fireslash
	float width;
	float cooldown;
	bool firebreath_is_active =false;

	//fireshield
	bool is_fireshield_on_cooldown = false;
	bool is_fireshield_up = false;
	float shield_activation_time = 300;
	float shield_max_time = 5000;
	float cooldown_shield = 10000;
	Timer fireshield_timer;

	//sprites
	SDL_Rect fireball_big;
	SDL_Rect fireball_small;

	Animation firebreath_right;
	Animation firebreath_left;

	Animation fireshield;
};

#endif