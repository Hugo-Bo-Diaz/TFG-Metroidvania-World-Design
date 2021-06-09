#ifndef HAZARD__SPIKES
#define HAZARD__SPIKES

#include "Physics.h"
#include "Timer.h"

class HazardSpikes :
	public physobj
{
public:
	HazardSpikes();

	bool Loop(float dt);
	bool Render();

	SDL_Rect spikes;
};

#endif
