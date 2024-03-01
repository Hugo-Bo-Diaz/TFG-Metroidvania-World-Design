#ifndef APPLICATION__H
#define APPLICATION__H

#include <list>
#include "PartsDef.h"
#include "Timer.h"

#include "pugiXML\src\pugixml.hpp"

#define BASEFPS 60

enum ApplicationState
{
	CREATE,
	LOOP,
	CLEAN,
	QUIT,
	EXIT
};


class Application
{
private:
	float dt;
	float fps_cap = 60;
	float last_frame_ms;
public:

	Input* inp;
	Window* win;
	Render* ren;
	Textures* tex;
	SceneController* scn;
	Physics* phy;
	Audio* aud;
	Camera* cam;
	Particles* par;
	UserInterface* gui;
	Text* txt;
	ProgressTracker* trk;
	Debug* dbg;

	void Run();

	Application(const char* aConfigFile, bool& aResult);
	std::string mConfigFile;

	bool Init();
	bool Loop();
	bool CleanUp();

	~Application() {};	
	std::list<Part*> parts;
	Timer update_timer;

	void LoadConfig(const char* filename);

	float GetLastUpdateTime() { return dt; }
};
extern Application* App;

#endif // !APPLICATION__H