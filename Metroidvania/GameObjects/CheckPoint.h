#ifndef CHECKPOINT__H
#define CHECKPOINT__H

#include "Physics.h"
class CheckPoint : public GameObject
{
public:
	CheckPoint();
	~CheckPoint();

	bool Loop(float dt);

	int room_id = 0;

	int spawn_pos_x = 0;
	int spawn_pos_y = 0;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new CheckPoint(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif