#ifndef CLOUD__SUMMONER__PROJECTILE__H
#define CLOUD__SUMMONER__PROJECTILE__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class CloudSummonerProjectile : public GameObject
{
public:

	CloudSummonerProjectile();
	~CloudSummonerProjectile();

	void Fire(float speed_x, float speed_y);

	float speed_x;
	float speed_y;

	bool Loop(float dt);
	bool Render();

	Animation projectile;

	int damage = 2;

	particle_preset magic;
	SDL_Rect r13magic;
};

#endif // !PLAYER__REP__H
