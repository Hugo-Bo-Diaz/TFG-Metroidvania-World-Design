#ifndef FLYING__ELEMENTAL__H
#define FLYING__ELEMENTAL__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"

enum FlyingElementalColor
{
	RED_FLYINGELEMENTAL
};
enum FlyingElementalState
{
	FE_PATROL,
	FE_STARTING_CHARGE,
	FE_CHARGE
};

class FlyingElemental : public Enemy
{
public:
	FlyingElemental();
	FlyingElemental(std::list<ObjectProperty*>& aProperties);
	FlyingElemental(float initial_y);
	~FlyingElemental() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	FlyingElementalState state = FE_PATROL;
	FlyingElementalState last_state = FE_PATROL;
	float health = 1;
	
	void RecieveDamage(int dmg, int direction);
	
	float x;
	float y;
	SDL_Rect* nextpos;

	float initial_y = -1;

	//graphics
	Animation flying_left;
	Animation flying_right;

	SDL_Rect r1exp;
	particle_preset explosion;

	void SetAnimations(FlyingElementalColor c);
	FlyingElementalColor c;

	//patrol
	float patrol_width = 48 * 3;
	float current_distance = 0;

	float patrol_accel_y = -0.5;
	float bounce_strength = 5;
	float patrol_speed=2;

	SDL_Rect aggro = {0,0,300,384};

	//starting

	float time_starting = 1000;
	Timer starting;
	float speed_starting = 2;

	//charge
	float charge_speed_x = 5;
	float charge_speed_y = 15;
	float charge_accel_y = -0.25;
	bool charged = false;
	
	//float charging_down_time = 5000;

	Timer charge_timer;

	//other
	float speed_x = patrol_speed;
	float speed_y = -10;
	float acceleration_y = patrol_accel_y;

	TextureID flyingelemental;
	TextureID particles;

	AudioID mSFXHit;
};

#endif