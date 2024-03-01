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

	Portal(std::list<ObjectProperty*>& aProperties) {

		for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
		{
			if ((*it)->name.compare("id_room") == 0)
			{
				id_destination_room = (*it)->num_value;
			}
			else if ((*it)->name.compare("id_point") == 0)
			{
				id_destination_point = (*it)->num_value;
			}
			else if ((*it)->name.compare("horizontal") == 0)
			{
				horizontal = (*it)->bool_value;
			}
		}
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

	SpawnPoint(std::list<ObjectProperty*>& aProperties) {
		for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
		{
			if ((*it)->name.compare("id") == 0)
			{
				id = (*it)->num_value;
			}
		}
	}
};




#endif // !PORTAL__H
