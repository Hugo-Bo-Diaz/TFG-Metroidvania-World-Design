#ifndef DEMO__END__OBJECT__H
#define DEMO__END__OBJECT__H

#include "Modules/ObjectManager.h"
#include "Modules/Gui.h"
#include "Modules/Textures.h"

class UItextbox;

class EndDemoObject :
	public GameObject
{
public:
	EndDemoObject();
	EndDemoObject(std::list<ObjectProperty*>&) { new (this) EndDemoObject; };

	void Init();
	bool Loop(float dt);
	bool Render();
	~EndDemoObject();

	std::string author = "DEVELOPER";

	std::vector<std::string> strings;
	UItextbox* textbox;

	void AddText(const char* text);

	TextBoxColor c = GREY;

	bool player_contact = false;
	bool textboxexists = false;
	int playerbooks = 0;

	SDL_Rect rectanglekeyboard;
	SDL_Rect rectanglecontroller;

	TextureID indicator;
	TextureID particles;
};

#endif