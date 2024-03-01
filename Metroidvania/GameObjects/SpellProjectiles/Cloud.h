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
	Cloud(std::list<ObjectProperty*>& aList) { new (this) Cloud; };

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
};

#endif // !PLAYER__REP__H