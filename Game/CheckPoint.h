#ifndef CHECKPOINT__H
#define CHECKPOINT__H

#include "Physics.h"
class CheckPoint : public physobj
{
public:
	CheckPoint();
	~CheckPoint();

	bool Loop(float dt);

	int room_id = 0;

	int spawn_pos_x = 0;
	int spawn_pos_y = 0;
};

#endif