#ifndef FIRST__DIALOGUE
#define FIRST__DIALOGUE
#include "Physics.h"

class UItextbox;

class FirstDialogue :
	public physobj
{
public:
	FirstDialogue();
	bool Loop(float dt);
	bool Render();

	UItextbox* t;
};

#endif