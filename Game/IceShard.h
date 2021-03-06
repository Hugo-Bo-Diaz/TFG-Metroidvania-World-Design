#ifndef ICE__SHARD__H
#define ICE__SHARD__H

#include "Physics.h"
#include "Animation.h"

class IceShard : public physobj
{
public:

	IceShard();
	~IceShard();

	void Fire(bool left_dir);

	bool Loop(float dt);

	float speed = 10;
	int direction;

	Animation ice_shard_left;
	Animation ice_shard_right;
};

#endif // !PLAYER__REP__H
