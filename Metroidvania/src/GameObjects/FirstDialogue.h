#ifndef FIRST__DIALOGUE
#define FIRST__DIALOGUE

#include "Modules/SceneController.h"
#include "Modules/Render.h"

class UItextbox;


class FirstDialogue :
	public GameObject
{
public:
	FirstDialogue();
	FirstDialogue(std::list<ObjectProperty*>&) { new (this) FirstDialogue; };

	void Init();
	bool Loop(float dt);
	bool Render();

	UItextbox* textbox;
	RexTextureID texture;
};

#endif