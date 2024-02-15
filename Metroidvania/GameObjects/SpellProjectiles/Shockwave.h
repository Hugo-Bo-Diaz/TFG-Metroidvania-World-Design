#ifndef SHOCKWAVE__H
#define SHOCKWAVE__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class Shockwave : public GameObject
{
public:

	Shockwave();
	~Shockwave();

	void Fire(bool left_dir, float speed);

	bool Loop(float dt);
	bool Render();

	float x_speed = 0;

	Animation shockwave_left;
	Animation shockwave_right;

	SDL_Point floor_check;

	ParticleEmitter* p;

	int damage = 2;
	int direction;

	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	SDL_Rect r8ground;
	SDL_Rect r9ground;
};

#endif // !PLAYER__REP__H
