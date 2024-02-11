#ifndef FLYING__SHIELD__H
#define FLYING__SHIELD__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"

class FlyingShield : public Enemy
{
public:
	FlyingShield();
	FlyingShield(float initial_y);
	~FlyingShield();

	bool Loop(float dt);
	bool Render();

	float health = 1;
	
	void RecieveDamage(int dmg, int direction);
	
	float x;
	float y;

	float initial_y;

	//graphics
	Animation flying_left;
	Animation flying_right;

	//patrol
	float patrol_width = 48 * 5;
	float current_distance = 0;

	float speed_x = 3;

};

#endif