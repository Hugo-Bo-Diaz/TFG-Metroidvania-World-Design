#ifndef HAZARD__SPIKES
#define HAZARD__SPIKES

#include "Modules/ObjectManager.h"
#include "Utils/Timer.h"
#include "Modules/Textures.h"

class HazardSpikes :
	public GameObject
{
public:
	HazardSpikes();
	HazardSpikes(std::list<ObjectProperty*>& aList) { new (this) HazardSpikes; };

	bool Loop(float dt);
	bool Render();

	SDL_Rect spikes;
	TextureID hazards;
};

#endif
