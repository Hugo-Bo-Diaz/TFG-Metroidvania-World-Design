#ifndef SHOCKWAVE__H
#define SHOCKWAVE__H

#include "Physics.h"
#include "Animation.h"

class ParticleEmitter;

class Shockwave : public physobj
{
public:

	Shockwave();

	void Fire(bool left_dir, float speed);

	bool Loop(float dt);
	bool Render();

	float x_speed = 0;

	Animation shockwave_left;
	Animation shockwave_right;

	SDL_Point floor_check;

	ParticleEmitter* p;

};

#endif // !PLAYER__REP__H
