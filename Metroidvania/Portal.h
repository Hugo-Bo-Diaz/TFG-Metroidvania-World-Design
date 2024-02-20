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

	static GameObject* Factory(std::list<ObjectProperty*>& aProperties) {
		Portal* portal = new Portal();

		for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
		{
			if ((*it)->name.compare("id_room") == 0)
			{
				portal->id_destination_room = (*it)->num_value;
			}
			else if ((*it)->name.compare("id_point") == 0)
			{
				portal->id_destination_point = (*it)->num_value;
			}
			else if ((*it)->name.compare("horizontal") == 0)
			{
				portal->horizontal = (*it)->bool_value;
			}
		}

		return portal;
	}
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

class SpawnPoint : public GameObject
{
public:
	int x;
	int y;
	int id;
	SpawnPoint() {

	}

	static GameObject* Factory(std::list<ObjectProperty*>& aProperties) {
		SpawnPoint* portal = new SpawnPoint();

		for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
		{
			if ((*it)->name.compare("id") == 0)
			{
				portal->id = (*it)->num_value;
			}
		}

		return portal;
	}
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};




#endif // !PORTAL__H
