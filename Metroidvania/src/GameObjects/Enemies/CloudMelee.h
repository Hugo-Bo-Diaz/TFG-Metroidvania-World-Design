#ifndef CLOUD__MELEE__H
#define CLOUD__MELEE__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

enum CloudMeleeState
{
	CM_PATROL,
	CM_STARTING_CHARGE,
	CM_CHARGE,
	CM_RECOVER
};

class CloudMelee : public Enemy
{
public:
	CloudMelee();
	CloudMelee(std::list<ObjectProperty*>& aList) { new (this) CloudMelee; };
	CloudMelee(float initial_x, float initial_y);
	~CloudMelee() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

	CloudMeleeState state = CM_PATROL;
	CloudMeleeState last_state = CM_PATROL;
	float health = 1;

	bool RecieveDamage(int dmg, int direction);

	float x;
	float y;

	float initial_y;
	float initial_x;
	RXRect* nextpos;

	//graphics
	Animation facing_left;
	Animation facing_right;
	Animation facing_front;

	RXRect r1exp;
	particle_preset explosion;

	//patrol
	float patrol_width = 48 * 3;
	float current_distance = 0;

	float patrol_accel_y = -0.5;
	float bounce_strength = 5;
	float patrol_speed = 2;

	RXRect aggro = { 0,-200,300,400 };

	//starting

	float time_starting = 1000;
	Timer starting;
	float starting_shake_max = 5;
	float starting_shake_x = 0;
	float starting_shake_y = 0;

	//charge
	float charge_speed_base = 10;
	float charge_speed_x = 10;
	float charge_speed_y = 10;

	bool charged = false;

	//float charging_down_time = 5000;

	Timer charge_timer;

	//other
	float speed_x = patrol_speed;
	float speed_y = -10;
	float acceleration_y = patrol_accel_y;

	RexTextureID cloud_melee;
	RexTextureID particles;

	AudioID mSFXHit;
};

#endif