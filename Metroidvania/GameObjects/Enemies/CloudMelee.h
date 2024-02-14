#ifndef CLOUD__MELEE__H
#define CLOUD__MELEE__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"

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
	CloudMelee(float initial_x, float initial_y);
	~CloudMelee();

	bool Loop(float dt);
	bool Render();

	CloudMeleeState state = CM_PATROL;
	CloudMeleeState last_state = CM_PATROL;
	float health = 1;

	void RecieveDamage(int dmg, int direction);

	float x;
	float y;

	float initial_y;
	float initial_x;

	//graphics
	Animation facing_left;
	Animation facing_right;
	Animation facing_front;

	//patrol
	float patrol_width = 48 * 3;
	float current_distance = 0;

	float patrol_accel_y = -0.5;
	float bounce_strength = 5;
	float patrol_speed = 2;

	SDL_Rect aggro = { 0,-200,300,400 };

	//starting

	float time_starting = 1000;
	Timer starting;
	float speed_starting = 2;

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

};

#endif