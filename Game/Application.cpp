#include "Application.h"
#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "SceneController.h"
#include "Physics.h"
#include "Console.h"
#include "Audio.h"
#include "Camera.h"
#include "Particles.h"
#include "Gui.h"
#include "Text.h"
#include "ProgressTracker.h"

#include <Windows.h>

bool Application::Init() 
{
#ifdef _DEBUG
	debug = true;
#endif _DEBUG

	if (debug)
	{
		ShowConsole();
	}
	else
	{
		HideConsole();
	}

	bool ret = true;
	inp = new Input();
	win = new Window();
	ren = new Render();
	tex = new Textures();
	scn = new SceneController();
	phy = new Physics();
	con = new Console();
	cam = new Camera();
	aud = new Audio();
	par = new Particles();
	gui = new UserInterface();
	txt = new Text();
	trk = new ProgressTracker();

	parts.push_back(con);
	parts.push_back(inp);
	parts.push_back(win);
	parts.push_back(phy);
	parts.push_back(gui);
	parts.push_back(scn);
	parts.push_back(cam);
	parts.push_back(par);
	parts.push_back(aud);
	parts.push_back(ren);
	parts.push_back(tex);
	parts.push_back(txt);
	parts.push_back(trk);

	LoadConfig("config.xml");
	

	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		if ((*it)->active)
		{
			if (!(*it)->Init())
			{
				ret = false;
			}
		}
	}
	return ret;
};

bool Application::Loop() 
{
	bool ret = true;
	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		if ((*it)->active)
		{
			if (!(*it)->Loop(dt))
			{
				ret = false;
			}
		}
	}


	return ret;
};
bool Application::CleanUp() 
{
	bool ret = true;
	for (std::list<Part*>::reverse_iterator it = parts.rbegin(); it != parts.rend(); it++)
	{
		if (!(*it)->CleanUp())
		{
			ret = false;
		}
	}
	return ret;
};

void Application::LoadConfig(const char* filename)
{
	bool create_file = false;
	pugi::xml_document	config_file;
	pugi::xml_node config_node;
	pugi::xml_parse_result result = config_file.load_file(filename);

	if (result == NULL)
	{
		printf("Could not load map xml file config.xml. pugi error: %s", result.description());
		printf("creating new configuration file...");

		create_file = true;
		config_node = config_file.append_child("config");
	}
	else
	{
		printf("found config file, loading...");
		config_node = config_file.child("config");
	}

	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{

		if (create_file == false)
		{
			pugi::xml_node part_node = config_node.child((*it)->name.c_str());
			(*it)->LoadConfig(part_node);
		}
		else
		{
			pugi::xml_node part_node = config_node.append_child((*it)->name.c_str());
			(*it)->CreateConfig(part_node);
		}
	}
	
	if (create_file)
	{
		config_file.save_file(filename);
	}
}


void Application::HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void Application::ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool Application::IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}