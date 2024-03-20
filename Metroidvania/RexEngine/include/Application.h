#ifndef APPLICATION__H
#define APPLICATION__H

#include <string>
#include <list>
#include "Modules/PartsDef.h"
#include "Utils/Timer.h"

#define BASEFPS 60

enum ApplicationState
{
	CREATE,
	LOOP,
	CLEAN,
	QUIT,
	EXIT
};

class EngineAPI;

class DLL_EXPORT Application
{
private:
	float dt;
	float fps_cap = 60;
	float last_frame_ms;
public:

	//thread blocking call until the engine has finished running
	void Run();

	//creates the application and loads its configuration
	Application(const char* aConfigFile, bool& aResult);
	std::string mConfigFile;

	//THIS SET OF FUNCTIONS IS CALLED AUTOMATICALLY, DO NOT CALL
	bool Init();
	bool Loop();
	bool CleanUp();
	//THIS SET OF FUNCTIONS IS CALLED AUTOMATICALLY, DO NOT CALL

	//other properties
	std::list<Part*> parts;
	Timer update_timer;
	void LoadConfig(const char* filename);
	float GetLastUpdateTime() { return dt; }

	EngineAPI* mAPI;
	friend class EngineAPI;
};

#endif // !APPLICATION__H