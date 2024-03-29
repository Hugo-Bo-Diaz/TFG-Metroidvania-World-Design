#ifndef ARMOR__TRAP__H
#define ARMOR__TRAP__H

#include "Physics.h"
#include "Animation.h"

class Player;

enum ArmorTrapState {
	ArmorTrap_IDLE,
	ArmorTrap_PATROL,
	ArmorTrap_CHASE
};

class ArmorTrap : public physobj
{
public:
	ArmorTrap();
	~ArmorTrap();

	bool Loop(float dt);
	bool Render();

	float patrol_speed = 1;
	float chase_speed = 3;
	float idle_speed = 0;
	float speed_x = patrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 4;

	void RecieveDamage(int dmg, int direction);

	float animation_interval_chase = 100;
	float animation_interval_patrol = 200;

	Animation left;
	Animation right;
	SDL_Rect idle;

	Timer animation_timer;

	bool knocked_up = false;

	physobj* target;

	ArmorTrapState current_state = ArmorTrap_IDLE;

	SDL_Rect aggro;

	bool isplayernearby;
};

#endif