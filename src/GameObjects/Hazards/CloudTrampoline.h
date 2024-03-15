#ifndef CLOUD__TRAMPOLINE__H
#define CLOUD__TRAMPOLINE__H

#include "Modules/ObjectManager.h"
#include "Utils/Timer.h"
#include "Modules/Textures.h"

class CloudTrampoline :
	public GameObject
{
public:
	CloudTrampoline();
	CloudTrampoline(std::list<ObjectProperty*>& aList) { new (this) CloudTrampoline; };

	void Init();
	bool Loop(float dt);
	bool Render();

	SDL_Rect cloudLeft;
	SDL_Rect cloudLoop;
	SDL_Rect cloudRight;

	TextureID hazards;
};

#endif
