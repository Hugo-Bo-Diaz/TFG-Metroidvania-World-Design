#ifndef ROCK__H
#define ROCK__H

#include "Physics.h"
#include "Animation.h"

class Rock : public physobj
{
public:

	Rock();
	~Rock();
	void Fire(bool left_dir, float angle, float speed, float gravity);

	bool Loop(float dt);
	bool Render();

	float x_speed = 0;
	float y_speed = 0;
	float gravity = 0;

	int damage = 4;
	int direction;
	
	Animation rock_sprite;
};

#endif // !PLAYER__REP__H
