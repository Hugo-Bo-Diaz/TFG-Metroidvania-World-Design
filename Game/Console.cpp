#include "Console.h"
#include "Application.h"

Debug::Debug()
{
	name = "Console";
}

bool Debug::Init()
{
	return true;
}

bool Debug::Loop(float dt)
{

	//system("CLS");
	/*printf("ACTIVE MODULES\n");
	printf("===================================================================\n");
	for (std::list<Part*>::iterator it = App->parts.begin(); it != App->parts.end(); it++)
	{
		if ((*it)->active)
			printf("%s\n",(*it)->name.c_str());
	}
	printf("===================================================================\n");
	printf("TRACKING VARIABLES\n");
	printf("===================================================================\n");
	for (std::list<var_track*>::iterator it = variables_tracked.begin(); it != variables_tracked.end(); it++)
	{
		printf("%s -> ", (*it)->name.c_str());
		int i = *((*it)->pointer_to_var);
		printf("%d\n", i);
	}
	printf("===================================================================\n");
	printf("LOGS\n");
	printf("===================================================================\n");
	for (std::list<console_log*>::iterator it = logs.begin(); it != logs.end(); it++)
	{
		printf("%s\n", (*it)->text.c_str());
		if ((*it)->timeout.Read_Sec() > MAX_TIME_LOG)
		{
			logs_to_delete.push_back(*it);
			//(*it)->todelete = true;
		}
	}
	*/
	//for (std::list<var_track*>::iterator it = variables_tracked_to_delete.begin(); it != variables_tracked_to_delete.end(); it++)
	//{
	//	delete(*it);
	//	variables_tracked.remove(*it);
	//}
	//variables_tracked_to_delete.clear();

	for (std::list<console_log*>::iterator it = logs_to_delete.begin(); it != logs_to_delete.end(); it++)
	{
		delete(*it);
		logs.remove(*it);
	}
	logs_to_delete.clear();


	return true;
}

void Debug::Console_log(char* log)
{
	console_log* l = new console_log();
	l->text = log;
	l->timeout.Start();

	logs.push_back(l);
}

var_track* Debug::AddVar_track(char* name, int* variable_pointer)
{
	var_track* t = new var_track();
	t->name = name;
	t->pointer_to_var = variable_pointer;

	variables_tracked.push_back(t);

	return t;
}

void Debug::DelVar_track(var_track* track)
{
	std::list<var_track*>::iterator t = std::find(variables_tracked.begin(), variables_tracked.end(), track);

	if (t != variables_tracked.end())
	{
		delete(*t);
		variables_tracked.remove(*t);
	}
	else if (track == *t)
	{
		delete(*t);
		variables_tracked.remove(*t);
	}
	else 
	{
		Console_log("tracking variable unable to be removed:");
	}
}

bool Debug::CleanUp()
{
	active = false;
	system("CLS");
	printf("closing main window / game\n");
	for (std::list<console_log*>::iterator it = logs.begin(); it != logs.end(); it++)
	{
		delete (*it);
	}
	logs.clear();

	return true;
}