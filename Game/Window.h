#ifndef WINDOW__H
#define WINDOW__H

#include"PartsDef.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Part
{
public:

	Window();
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void SetWindowTitle(const char* title);
	float GetScale() { return (float)scale; }
	void ToggleFullScreen();

	int		width;
	int		height;
	float	scale;

	bool fullscreen = false;
	bool borderless = false;
	bool resizable = false;
	bool fullscreen_window = false;
	
	float base_window_width = 1024;
	float base_window_height = 576;

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

public:

	SDL_Window* window;
	SDL_Surface* screen_surface;

};

#endif // !WINDOW__H
