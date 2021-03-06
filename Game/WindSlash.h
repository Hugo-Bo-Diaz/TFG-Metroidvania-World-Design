#ifndef WIND__SLASH__H
#define WIND__SLASH__H

#include "Physics.h"
#include "Animation.h"

class WindSlash : public physobj
{
public:

	WindSlash();
	~WindSlash();

	void Fire(bool left_dir);

	bool Loop(float dt);

	float speed = 20;
	int direction;

	Animation windslash;
};

#endif // !PLAYER__REP__H
