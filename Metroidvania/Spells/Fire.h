#ifndef FIRE__H
#define FIRE__H

#include "Spell.h"
#include "Animation.h"
#include "Timer.h"
#include "ParticleEmitter.h"

class ParticleEmitter;
class LavaSpell;

class Fire : public Spell
{
public:

	~Fire();

	void Init();

	void Loop(float dt);
	void Render();

	void Switched_in();
	void Switched_out();

	void UnlockMovementEvent();

	void CleanUp();

	//fireball
	int charge = 0;
	float manacost_small = 0.25;
	float manacost_big = 0.5;

	//firevolcano
	float width;
	bool firevolcano_is_active =false;
	std::vector<LavaSpell*> lavalist;
	std::vector<LavaSpell*> to_delete_lavalist;
	void DeleteLava(LavaSpell*);
	Timer lavaspawner;
	float timebetweenlava = 100;
	ParticleEmitter* volcano_particles;
	bool is_volcano_active = false;
	int audiochannel_volcano = 10;
	float manacost_volc = 0.2;

	//fireshield
	bool is_fireshield_on_cooldown = false;
	bool is_fireshield_up = false;
	float shield_activation_time = 300;
	float shield_max_time = 5000;
	float cooldown_shield = 5750;
	Timer fireshield_timer;
	
	ParticleEmitter*to_follow;
	particle_preset fireshield_part;
	SDL_Rect r2shield;
	SDL_Rect r12shield;

	particle_preset lava;
	SDL_Rect r3lava;


	int audiochannel_shield = 0;
	float manacost_shield = 0.25;
	float manacost_shield_over_time = 0.005;

	//sprites
	SDL_Rect fireball_big;
	SDL_Rect fireball_small;

	Animation firebreath_right;
	Animation firebreath_left;

	Animation fireshield;
};


#endif