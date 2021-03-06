#ifndef PLAYER__H
#define PLAYER__H

#include "Physics.h"
#include "Console.h"
#include "Animation.h"

#include "Spell.h"

enum state
{
IDLE,

WALKING_LEFT,
WALKING_RIGHT,

AIRBORNE_LEFT,
AIRBORNE_RIGHT,

LAG
};

enum spell_type
{
	FIRE,
	WIND,
	WATER,
	GRASS,
	GROUND,
};

class Player : public physobj
{
public:
	Player();
	~Player();
	state current_state = IDLE;
	bool Loop(float dt);
	
	int x = 0;
	int y = 0;

	float speed_x = 0;
	float speed_y = -100;
	
	int spd=0;

	float acceleration_x = 0;
	float acceleration_y = 1.0;

	bool grounded = true;

	void LockMovement(float time_in_ms =-1);
	float time_to_lock_movement=0;
	bool movement_is_locked = false;
	void UnlockMovement();
	Timer movement_lock;

	//STATS
	float max_speed = 6.0;
	float accel_x = 1;

	float mobility_multiplier = 1.0f;

	bool is_right = true;

	Animation idle_right;
	Animation idle_left;
	Animation walking_right;
	Animation walking_left;
	Animation air_right;
	Animation air_left;

	var_track* pos_x;
	var_track* pos_y;

	var_track* xspeed;

	bool left;
	bool right;
	bool jump;
	bool stop;

	SDL_Point trail[30];

	spell_type current_spell=FIRE;
	std::vector<Spell*> spells;
};

#endif // !PLAYER__H
