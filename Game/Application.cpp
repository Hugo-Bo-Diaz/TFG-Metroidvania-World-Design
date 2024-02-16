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

#include <Windows.h>
#include <exception>

#include "SDL/include/SDL.h"

Application::Application()
{
	SDL_Init(SDL_INIT_VIDEO);
}


void ExceptionHandler()
{
	std::exception_ptr lExc = std::current_exception();

	try
	{
		std::rethrow_exception(lExc);
	}
	catch (const std::exception & e)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, e.what());
	}
}


void Application::Run() {
	ApplicationState state = CREATE;

	state = LOOP;
	Start();

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
	cam = new Camera();
	aud = new Audio();
	par = new Particles();
	gui = new UserInterface();
	txt = new Text();
	trk = new ProgressTracker();

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

bool Application::Start()
{
	std::set_terminate(ExceptionHandler);

	bool ret = true;
	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		if (!(*it)->Start())
		{
			ret = false;
		}
	}

	return ret;

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
	bool ret = true;
	for (std::list<Part*>::reverse_iterator it = parts.rbegin(); it != parts.rend(); it++)
	{
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

	if (result == NULL)
	{
		std::string errstr = "Could not load map xml file config.xml. pugi error: ";
		errstr += result.description();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
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
		Logger::Console_log(LogLevel::LOG_INFO,"found config file, loading...");
		config_node = config_file.child("config");
	}

	for (std::list<Part*>::iterator it = parts.begin(); it != parts.end(); it++)
	{
			pugi::xml_node part_node = config_node.child((*it)->name.c_str());
			(*it)->LoadConfig(part_node);
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