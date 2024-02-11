#ifndef DEMO__END__OBJECT__H
#define DEMO__END__OBJECT__H
#include "Physics.h"
#include "Gui.h"
class UItextbox;

class EndDemoObject :
	public GameObject
{
public:
	EndDemoObject();
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
};

#endif