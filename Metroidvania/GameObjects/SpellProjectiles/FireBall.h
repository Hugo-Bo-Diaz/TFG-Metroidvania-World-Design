#ifndef FIRE__BALL__H
#define FIRE__BALL__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class FireBall : public GameObject
{
public:

	FireBall();

	void Destroy();


	void Fire(bool left_dir, bool is_big);

	bool Loop(float dt);
	bool Render();

	float speed = 15;
	int direction;

	bool is_big = false;

	Animation fireball_small;
	Animation fireball_big;

	SDL_Rect r1exp;
	particle_preset explosion;

	int damage = 2;
};

#endif // !PLAYER__REP__H
