#ifndef CLOUD__SUMMONER__PROJECTILE__H
#define CLOUD__SUMMONER__PROJECTILE__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"
#include "Textures.h"

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

	TextureID cloud_summoner;
	TextureID particles;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new CloudSummonerProjectile(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H
