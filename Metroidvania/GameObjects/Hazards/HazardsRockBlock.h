#ifndef HAZARD__ROCK__BLOCK
#define HAZARD__ROCK__BLOCK

#include "Physics.h"
#include "Timer.h"
#include "Textures.h"

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
	TextureID hazards;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new HazardRockBlock(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif
