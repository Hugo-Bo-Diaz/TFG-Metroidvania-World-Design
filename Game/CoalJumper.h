#ifndef COALJUMPER__H
#define COALJUMPER__H

#include "Physics.h"
#include "Animation.h"


enum CoalJumperState {
	COALJUMPER_IDLE,
	COALJUMPER_CROUCHING,
	COALJUMPER_JUMPING,
	COALJUMPER_LANDING,

	COALJUMPER_HIT,

	MAX_COAL_JUMPER_STATES
};

class CoalJumper : public physobj
{
public:
	CoalJumper();
	~CoalJumper();

	bool Loop(float dt);
	bool Render();

	float health=5;
	void RecieveDamage(int dmg, int direction);

	//Idle
	float time_between_frames = 200;
	Timer animation_timer;
	float idle_time = 1000;
	Timer idle_timer;
	int max_variation = 2000;

	//Crouching
	float time_crouching = 2000;
	Timer crouching;

	//Jumping
	float speed_x = 0;
	float speed_while_jumping=5;
	float speed_y = 0;
	float jump_strength = -18;
	float acceleration_y = 1;
	int direction = 1;
	Timer jumping_animation_timer;

	//landing
	float cooldown = 1000;
	Timer cooldown_timer;

	//recieved knockback
	float speed_y_knockback = -10;
	float speed_x_knockback = 5;

	//other
	int jumps_to_side = 0;
	int max_jumps_to_side = 1;
	CoalJumperState state = COALJUMPER_IDLE;
	CoalJumperState last_state = COALJUMPER_IDLE;
	Animation animations[MAX_COAL_JUMPER_STATES];
};

#endif