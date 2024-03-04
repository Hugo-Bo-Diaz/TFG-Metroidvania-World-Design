#ifndef HAZARD__ROCK__BLOCK
#define HAZARD__ROCK__BLOCK

#include "Modules/ObjectManager.h"
#include "Utils/Timer.h"
#include "Modules/Textures.h"

class HazardRockBlock :
	public GameObject
{
public:
	HazardRockBlock();
	HazardRockBlock(std::list<ObjectProperty*>& aList) { new (this) HazardRockBlock; };
	~HazardRockBlock();
	int wall_id;

	void Init();
	bool Loop(float dt);
	bool Render();

	SDL_Rect rockblock;
	TextureID hazards;
};

#endif
