#ifndef CLOUD__TRAMPOLINE__H
#define CLOUD__TRAMPOLINE__H

#include "Modules/SceneController.h"
#include "Utils/Timer.h"
#include "Modules/Render.h"

class CloudTrampoline :
	public GameObject
{
public:
	CloudTrampoline();
	CloudTrampoline(std::list<ObjectProperty*>& aList) { new (this) CloudTrampoline; };

	void Init();
	bool Loop(float dt);
	bool Render();

	RXRect cloudLeft;
	RXRect cloudLoop;
	RXRect cloudRight;

	RexTextureID hazards;
};

#endif
