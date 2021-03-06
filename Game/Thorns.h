#ifndef THORNS__H
#define THORNS__H

#include "Physics.h"

enum ThornsState {
	SEED,
	THORNS_ON_FLOOR
};

class Thorns : public physobj
{
public:

	Thorns() { state = SEED; };
	~Thorns() {};

	void Fire(bool left_dir, float lifespan);

	bool Loop(float dt);

	float speed = 5;
	float y_speed = -5;
	float gravity = 0.5;
	int direction;

	float expansion_speed = 8;

	float lifespan = 5000;
	Timer life_timer;

	ThornsState state = SEED;

	SDL_Rect thorn_first = {0,0,32,32};
	SDL_Rect thorn_second = {32,0,32,32};
	SDL_Rect seed = {64,0,32,32};
	SDL_Rect last_thorn_segment = { 0,0,8,32 };
};

#endif // !PLAYER__REP__H
