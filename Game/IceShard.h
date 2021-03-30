#ifndef ICE__SHARD__H
#define ICE__SHARD__H

#include "Physics.h"
#include "Animation.h"

class ParticleEmitter;

class IceShard : public physobj
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
};

#endif // !PLAYER__REP__H
