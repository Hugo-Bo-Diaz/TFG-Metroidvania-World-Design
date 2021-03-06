#ifndef LEAF__H
#define LEAF__H

#include "Physics.h"
#include "Animation.h"

class Leaf : public physobj
{
public:

	Leaf();
	~Leaf();

	void Fire(bool left_dir, float angle);

	bool Loop(float dt);

	float speed = 15;
	int direction;

	float angle;
	float ratio_x;
	float ratio_y;

	Animation leaf_left;
	Animation leaf_right;
};

#endif // !PLAYER__REP__H
