#ifndef APPLICATION__H
#define APPLICATION__H

#include <list>
#include "PartsDef.h"
#include "Timer.h"

#include "pugiXML\src\pugixml.hpp"


class Application
{
public:

	Input* inp;
	Window* win;
	Render* ren;
	Textures* tex;
	SceneController* scn;
	Physics* phy;
	Console* con;
	Audio* aud;
	Camera* cam;
	Particles* par;
	UserInterface* gui;
	Text* txt;

	Application() {};

	bool Init();
	bool Loop();
	bool CleanUp();

	~Application() {};	
	std::list<Part*> parts;
	Timer update_timer;

	void LoadConfig(const char* filename);

private:

	float dt;


};
extern Application* App;

#endif // !APPLICATION__H