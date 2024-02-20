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
	bool Loop(float dt);
	bool Render();

	UItextbox* textbox;
	TextureID texture;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new FirstDialogue(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif