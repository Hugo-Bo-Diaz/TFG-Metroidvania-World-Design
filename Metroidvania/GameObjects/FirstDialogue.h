#ifndef FIRST__DIALOGUE
#define FIRST__DIALOGUE

#include "Physics.h"
#include "Textures.h"

class UItextbox;


class FirstDialogue :
	public GameObject
{
public:
	FirstDialogue();
	FirstDialogue(std::list<ObjectProperty*>&) { new (this) FirstDialogue; };

	bool Loop(float dt);
	bool Render();

	UItextbox* textbox;
	TextureID texture;
};

#endif