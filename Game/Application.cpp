#include "Application.h"
#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "SceneController.h"
#include "Physics.h"
#include "Logger.h"
#include "Audio.h"
#include "Camera.h"
#include "Particles.h"
#include "Gui.h"
#include "Text.h"
#include "ProgressTracker.h"
#include "Debug.h"

#include <exception>
#include <signal.h>
#include <iostream>
#include <string>
#include <sstream>
#include "SDL/include/SDL.h"

void ExceptionHandler(int signal_hand)
{
	switch (signal_hand)
	{
	case SIGABRT:
		Logger::Console_log(LogLevel::LOG_ERROR, "Anomalous termination");
		break;
	case SIGFPE:
		Logger::Console_log(LogLevel::LOG_ERROR, "Floating point exception");
		break;
	case SIGILL:
		Logger::Console_log(LogLevel::LOG_ERROR, "Non-valid instruction");
		break;
	case SIGSEGV:
		Logger::Console_log(LogLevel::LOG_ERROR, "Non-valid access to pointer");
		break;
	case SIGTERM:
		Logger::Console_log(LogLevel::LOG_ERROR, "Solicited termination");
		break;
	default:
		Logger::Console_log(LogLevel::LOG_ERROR, "Unknown error!");
		break;
	}
}

Application::Application(const char* aConfigFile, bool& aSuccesful)
{
	mConfigFile = aConfigFile;
	aSuccesful = true;

	SDL_Init(SDL_INIT_VIDEO);

	signal(SIGBREAK, ExceptionHandler);
	signal(SIGABRT, ExceptionHandler);
	signal(SIGFPE, ExceptionHandler);
	signal(SIGILL, ExceptionHandler);
	signal(SIGSEGV, ExceptionHandler);

	inp = new Input();
	win = new Window();
	ren = new Render();
	tex = new Textures();
	scn = new SceneController();
	phy = new Physics();
	cam = new Camera();
	aud = new Audio();
	par = new Particles();
	gui = new UserInterface();
	txt = new Text();
	trk = new ProgressTracker();
	dbg = new Debug();

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
	parts.push_back(dbg);
}


void Application::Run()
{
	ApplicationState state = CREATE;
	if (!Init())
	{
		state = EXIT;
		Logger::Console_log(LogLevel::LOG_ERROR, "Could not initialize engine!");
	}
	state = LOOP;

	while (state != EXIT)
	{
		switch (state)
		{
		case LOOP:
		{
			if (!App->Loop())
			{
				state = QUIT;
				Logger::Console_log(LogLevel::LOG_ERROR,"EXITING: with errors (loop)");
			}
		}
		break;
		case QUIT:
		{
			if (!App->CleanUp())
			{
				Logger::Console_log(LogLevel::LOG_ERROR, "EXITING: with errors (cleanup)");
			}
			else
			{
				delete App;
				App = nullptr;
				Logger::Console_log(LogLevel::LOG_ERROR,"EXITING: no errors found");
			}
			state = EXIT;
		}
		break;
		default:
			printf("dafuq\n");
			break;
		}
	}
}



bool Application::Init()
{
	Logger::Console_log(LogLevel::LOG_INFO, "Starting engine");

	LoadConfig(mConfigFile.c_str());

	Logger::Console_log(LogLevel::LOG_INFO, "Initializing engine");
	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		if ((*it)->active)
		{
			std::ostringstream lStr;
			lStr << "Initializing" << (*it)->name;
			Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());
			if (!(*it)->Init())
			{
				return false;
			}
		}
	}
	return true;
}

bool Application::Loop() 
{
	dt = update_timer.Read();
	update_timer.Reset();

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
	
	float ms_of_frame = (1 / fps_cap) * 1000;

	if (fps_cap != 0)
	{
		float time_left_of_the_frame = ms_of_frame - update_timer.Read();
		if (time_left_of_the_frame > 0)
		{
			SDL_Delay(time_left_of_the_frame);
		}
	}

	float base_ms_on_frame = (1000 / 60);
	dt =1/( base_ms_on_frame/ms_of_frame);
	return ret;
};

bool Application::CleanUp() 
{
	Logger::Console_log(LogLevel::LOG_INFO, "Closing the engine");

	bool ret = true;
	for (std::list<Part*>::reverse_iterator it = parts.rbegin(); it != parts.rend(); it++)
	{
		std::ostringstream lStr;
		lStr << "Cleaning up" << (*it)->name;
		Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());
		if (!(*it)->CleanUp())
		{
			ret = false;
		}
	}
	SDL_Quit();

	return ret;
};

void Application::LoadConfig(const char* filename)
{
	pugi::xml_document	config_file;
	pugi::xml_node config_node;
	pugi::xml_parse_result result = config_file.load_file(filename);

	if (result.status != pugi::xml_parse_status::status_ok)
	{
		std::ostringstream lStr;
		lStr << "Could not load config xml file"<< filename << "pugi error: " << result.description();
		Logger::Console_log(LogLevel::LOG_ERROR, lStr.str().c_str());
		Logger::Console_log(LogLevel::LOG_WARN,"creating new configuration file...");

		config_node = config_file.append_child("config");
		for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
		{
			pugi::xml_node part_node = config_node.append_child((*it)->name.c_str());
			(*it)->CreateConfig(part_node);
		}
		config_file.save_file(filename);
	}
	else
	{
		std::ostringstream lStr;
		lStr << "found config file,"<< filename << "loading the engine with config...";
		Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());
		config_node = config_file.child("config");
	}

	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
			pugi::xml_node part_node = config_node.child((*it)->name.c_str());
			(*it)->LoadConfig(part_node);
	}
}