#include "Window.h"
#include "SDL_image/include/SDL_image.h"

#include "SDL/include/SDL.h"

Window::Window()
{
	name = "Window";

	window = NULL;
	screen_surface = NULL;
}

bool Window::LoadConfig(pugi::xml_node& config_node)
{
	printf("Init SDL window & surface\n");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		Uint32 flags = SDL_WINDOW_SHOWN;

		pugi::xml_node dimension_node = config_node.child("dimensions");
		width = dimension_node.attribute("width").as_int(1024);
		height = dimension_node.attribute("height").as_int(576);//PIXEL ART RATIO = 2
		scale = dimension_node.attribute("scale").as_int(1);

		if (config_node.child("fullscreen").attribute("value").as_bool())
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			fullscreen = true;
		}

		if (config_node.child("borderless").attribute("value").as_bool())
		{
			flags |= SDL_WINDOW_BORDERLESS;
			borderless = true;
		}

		if (config_node.child("resizable").attribute("value").as_bool())
		{
			flags |= SDL_WINDOW_RESIZABLE;
			resizable = true;
		}

		if (config_node.child("fullscreen_window").attribute("value").as_bool())
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			fullscreen_window = true;
		}

		std::string title = config_node.child("window_title").attribute("value").as_string("DEFAULT_CAPTION");
		
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return true;
}
bool Window::CreateConfig(pugi::xml_node& config_node)
{
	printf("Init SDL window & surface\n");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		Uint32 flags = SDL_WINDOW_SHOWN;

		width = 1024;
		height = 576;//PIXEL ART RATIO = 4
		scale = 1;
		pugi::xml_node dimensions = config_node.append_child("dimensions");

		dimensions.append_attribute("width") = width;
		dimensions.append_attribute("height") = height;
		dimensions.append_attribute("scale") = scale;

		config_node.append_child("fullscreen").append_attribute("value") = false;
		config_node.append_child("borderless").append_attribute("value") = false;
		config_node.append_child("resizable").append_attribute("value") = false;
		config_node.append_child("fullscreen_window").append_attribute("value") = false;

		config_node.append_child("window_title").append_attribute("value") = "DEFAULT_CAPTION";

		window = SDL_CreateWindow("DEFAULT CAPTION", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return true;
}

bool Window::Init()
{
	bool ret = true;
	

	//SET THE ICON OF THE WINDOW HERE 

	return ret;

}
bool Window::Loop(float dt)
{
	int window_x, window_y;
	SDL_GetWindowSize(window, &width, &height);

	scale = height / base_window_height;
	
	//SDL_SetWindowSize(window, height * 16 / 9, height);
	//SDL_SetWindowSize(window, 1280, 720);
	return true;
}

void Window::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void Window::ToggleFullScreen()
{
	if(fullscreen)
	{
		SDL_SetWindowFullscreen(window,0);
		fullscreen = false;
		scale = 1;
	}
	else
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fullscreen = true;
		scale = 2;

	}
}

bool Window::CleanUp()
{
	bool ret = true;

	printf("Quitting Window\n");
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	active = false;
	return ret;
}