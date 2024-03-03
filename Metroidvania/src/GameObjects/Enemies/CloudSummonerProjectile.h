#ifndef CLOUD__SUMMONER__PROJECTILE__H
#define CLOUD__SUMMONER__PROJECTILE__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class CloudSummonerProjectile : public GameObject
{
public:

	CloudSummonerProjectile();
	CloudSummonerProjectile(std::list<ObjectProperty*>& aList) { new (this) CloudSummonerProjectile; };
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

	TextureID cloud_summoner;
	TextureID particles;
};

#endif // !PLAYER__REP__H
