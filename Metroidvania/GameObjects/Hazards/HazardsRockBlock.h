#ifndef HAZARD__ROCK__BLOCK
#define HAZARD__ROCK__BLOCK

#include "Physics.h"
#include "Timer.h"

class HazardRockBlock :
	public GameObject
{
public:
	HazardRockBlock();
	~HazardRockBlock();
	void Init();
	int wall_id;

	bool Loop(float dt);
	bool Render();

	SDL_Rect rockblock;
};

#endif
