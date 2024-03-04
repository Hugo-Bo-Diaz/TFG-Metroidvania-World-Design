#ifndef ICE__SHARD__H
#define ICE__SHARD__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"

class ParticleEmitter;

class IceShard : public GameObject
{
public:

	IceShard();
	IceShard(std::list<ObjectProperty*>& aList) { new (this) IceShard; };

	void Fire(bool left_dir);

	void Init();
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
};

#endif // !PLAYER__REP__H
