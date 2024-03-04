#ifndef FLYING__AXE__H
#define FLYING__AXE__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

enum FlyingAxeState
{
	AXE_PATROL,
	AXE_STARTING_CHARGE,
	AXE_CHARGE
};

class FlyingAxe : public Enemy
{
public:
	FlyingAxe();
	FlyingAxe(std::list<ObjectProperty*>& aList) { new (this) FlyingAxe; };
	FlyingAxe(float initial_y);
	~FlyingAxe();

	void Init();
	bool Loop(float dt);
	bool Render();

	FlyingAxeState state = AXE_PATROL;
	FlyingAxeState last_state = AXE_PATROL;
	float health = 1;
	
	void RecieveDamage(int dmg, int direction);
	
	float x;
	float y;

	float initial_y;
	SDL_Rect* nextpos = nullptr;

	Timer animation_timer;

	//graphics
	Animation rotate_left;
	Animation rotate_right;

	particle_preset metal;
	SDL_Rect r18metalfirst;
	SDL_Rect r19metalsecond;

	SDL_Rect facing_left;
	SDL_Rect facing_right;

	float animation_frequency = 100;

	//patrol
	float patrol_width = 48 * 3;
	float current_distance = 0;
	float patrol_speed = 2;

	SDL_Rect aggro = {0,0,300,384};

	//starting

	Timer starting;
	float time_starting = 1000;

	//charge
	float charge_speed = 15;
	float charge_speed_x = 0;
	float charge_speed_y = 0;
	bool charged = false;
	//other
	float speed_x = patrol_speed;
	float speed_y = 0;

	TextureID floating_axe;
	TextureID particles;
};

#endif