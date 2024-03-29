#ifndef SHIELD__MONSTER__H
#define SHIELD__MONSTER__H

#include "Physics.h"
#include "Animation.h"

class Player;

enum ShieldMonsterState {
	ShieldMonster_PATROL,
	ShieldMonster_CHASE,
	ShieldMonster_WINDUP,
	ShieldMonster_ATTACK
};

class ShieldMonster : public physobj
{
public:
	ShieldMonster();
	~ShieldMonster();

	bool Loop(float dt);
	bool Render();


	//patrol
	float partrol_speed = 1;
	SDL_Rect aggro;
	bool isplayernearby;
	float animation_interval_patrol = 200;
	float patrol_range = 100;
	int distance_traveled;


	//chase
	float chase_speed = 2;
	SDL_Rect range;
	bool isplayeronrange;
	float animation_interval_chase = 100;

	//windup
	float max_windupangle = 90;
	float winduptime = 1000;
	Timer windupTimer;

	//attack
	float attacktime = 100;
	Timer attackTimer;
	float totalanimation = 500;

	float speed_x = partrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 10;

	void RecieveDamage(int dmg, int direction);

	SDL_Rect shield;
	float max_shield_height = 140;

	SDL_Rect HitBox;
	bool hashitground = false;

	Animation left;
	Animation right;
	SDL_Rect arm_left;
	SDL_Rect arm_right;
	float arm_angle = 0;
	int gdirection = 0;

	Timer animation_timer;

	bool knocked_up = false;

	physobj* target;

	ShieldMonsterState current_state = ShieldMonster_PATROL;

	float realx;


};

#endif