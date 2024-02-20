#ifndef ICE__SHARD__H
#define ICE__SHARD__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class IceShard : public GameObject
{
public:

	IceShard();

	void Fire(bool left_dir);

	bool Loop(float dt);
	bool Render();

	float speed = 10;
	int direction;

	Animation ice_shard_left;
	Animation ice_shard_right;

	ParticleEmitter* p;
	particle_preset ice;
	SDL_Rect r4ice;

	TextureID spells;
	TextureID particles;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new IceShard(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H
