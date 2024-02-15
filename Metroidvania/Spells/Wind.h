#ifndef WIND__H
#define WIND__H

#include "Spell.h"
#include "Animation.h"
#include "Timer.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class Wind : public Spell
{
public:

	~Wind();

	void Init();

	void Loop(float dt);

	void UnlockMovementEvent();

	void Switched_in();
	void Switched_out();

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
	float cooldown_mobility = 5750;
	Timer mobility_timer;

	ParticleEmitter* p;
	particle_preset windbuff;
	SDL_Rect r6buff;
	SDL_Rect r7buff;
};

#endif