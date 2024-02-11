#ifndef PORTAL__H
#define PORTAL__H

#include "Physics.h"

class Portal : public GameObject
{
public:
	SDL_Rect area;
	int id_destination_room;
	int id_destination_point;
	bool horizontal;
	Portal() {

	}
};

class SpawnPoint : public GameObject
{
public:
	int x;
	int y;
	int id;
	SpawnPoint() {

	}
};




#endif // !PORTAL__H
