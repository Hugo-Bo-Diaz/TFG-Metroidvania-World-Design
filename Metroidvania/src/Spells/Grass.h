#ifndef GRASS__H
#define GRASS__H

#include "Spell.h"
#include "EngineElements/Animation.h"
#include "Utils/Timer.h"
#include "EngineElements/ParticleEmitter.h"

class ParticleEmitter;

class Grass : public Spell
{
public:
	Grass(EngineAPI* aAPI) : Spell(aAPI) {};
	~Grass();

	void Init();

	void Loop(float dt);
	void Render();

	void Switched_in();
	void Switched_out();

	void UnlockMovementEvent();

	void CleanUp();

	//leafshot
	int charge = 0;
	int max_charge = 100;
	float min_angle= 10;
	float max_angle = 60;


	//grasshook
	float distance;
	float max_distance = 320;
	float speed_hook = 15;
	float speed_player = 10;
	bool hooked = false;
	bool hook_out = false;
	float hook_position_x = 0;
	float hook_position_y = 0;
	float current_angle = 0;
	float draw_angle = 0;
	RXRect t = { 64,0,64,32 };

	RXRect debug = { 0,0,10,10 };
	
	//thorns terrain
	bool is_thorns_on_cooldown = false;
	float thorns_max_time = 5000;
	float cooldown_thorns = 5000;
	Timer thorns_timer;

	//sprites
	RXRect leaf = { 0,0,64,32 };
	RXRect vine = {160,32,32,32};
	RXRect hook = {192,32,32,32};
	ParticleEmitter*p;

	particle_preset grass;
	RXRect r10grass;
	RXRect r11grass;

	TextureID spells;
	TextureID particles;
};

#endif