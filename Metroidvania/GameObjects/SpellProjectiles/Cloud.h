#ifndef CLOUD__H
#define CLOUD__H

#include "Physics.h"
#include "Animation.h"
#include "Textures.h"
#include <vector>
class Rain;

class Cloud : public GameObject
{
public:

	Cloud();

	bool Loop(float dt);
	bool Render();

	Timer timer;
	float life_expectancy = 5000;

	float rain_frequency = 80;
	float rain_speed=4;
	Timer rain_timer;

	void DeleteRainDrop(Rain* drop);

	SDL_Rect hitbox;

	SDL_Rect blitrect;

	std::vector<Rain*> rain_vector;
	std::vector<Rain*> rain_to_delete;

	TextureID spells;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new Cloud(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H