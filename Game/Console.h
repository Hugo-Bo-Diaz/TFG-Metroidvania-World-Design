#ifndef CONSOLE__H
#define CONSOLE__H

#include "PartsDef.h"
#include "Timer.h"
#include <list>

#define MAX_TIME_LOG 2 // seconds

class console_log
{
public:
	Timer timeout;
	std::string text;
	//bool todelete = false;
};

struct var_track
{
	std::string name;
	int* pointer_to_var;
};

class Debug : public Part
{
public:

	Debug();

	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	
	void Console_log(char* log);

	var_track* AddVar_track(char* name, int* variable_pointer);
	void DelVar_track(var_track* del);

private:
	int i=0;
	std::list<console_log*> logs;
	std::list<console_log*> logs_to_delete;

	std::list<var_track*> variables_tracked;
	std::list<var_track*> variables_tracked_to_delete;


};

#endif //CONSOLE__H