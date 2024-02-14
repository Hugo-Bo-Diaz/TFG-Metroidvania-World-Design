#ifndef WIND__SLASH__H
#define WIND__SLASH__H

#include "Physics.h"
#include "Animation.h"

class ParticleEmitter;

class WindSlash : public GameObject
{
public:

	WindSlash();

	void Fire(bool left_dir);

	bool Loop(float dt);
	bool Render();

	float speed = 20;
	int direction;

	Animation windslash;

	ParticleEmitter* p;
};

#endif // !PLAYER__REP__H