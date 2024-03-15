#ifndef GROUND__H
#define GROUND__H

#include "Spell.h"
#include "EngineElements/Animation.h"
#include "Utils/Timer.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

class Ground : public Spell
{
public:
	Ground(EngineAPI* aAPI) :Spell(aAPI) {};

	void Init();

	void Loop(float dt);
	void Render();

	void Switched_in();
	void Switched_out();

	void UnlockMovementEvent();

	void CleanUp();

	//rock throw
	float rock_speed_x = 10;
	float rock_speed_y = -5;
	float rock_gravity = 0.5;
	
	float cooldown_rock = 150;
	Timer rock_timer;
	bool is_rock_on_cooldown = false;
	float manacost_rock = 0.3;

	//ground pound
	float initial_yspeed = -5;
	float gravity = 2;
	float current_yspeed = 0;

	bool groundpounding = false;
	bool is_on_gp_lag = false;
	float groundpound_endlag = 200;
	Timer groundpound_end_timer;
	float manacost_groundpound = 1.0;

	//earthquake
	float cooldown_earthquake = 500;
	bool is_eq_on_cooldown = false;
	Timer earthquake_timer;

	RXRect groundpoundhitbox;
	bool hashitsomething;

	float manacost_earthquake = 0.5;

	//sprites
	RXRect groundpound = { 128,160,64,32 };

	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	RXRect r8ground;
	RXRect r9ground;

	TextureID spells;
	TextureID particles;

	AudioID mSFXGroundPound;
	AudioID mSFXRock;
	AudioID mSFXShockwave;
};

#endif