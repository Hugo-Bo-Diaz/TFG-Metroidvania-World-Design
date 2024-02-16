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
};

#endif
