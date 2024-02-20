#ifndef HAZARD__SPIKES
#define HAZARD__SPIKES

#include "Physics.h"
#include "Timer.h"
#include "Textures.h"

class HazardSpikes :
	public GameObject
{
public:
	HazardSpikes();

	bool Loop(float dt);
	bool Render();

	SDL_Rect spikes;
	TextureID hazards;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new HazardSpikes(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif
