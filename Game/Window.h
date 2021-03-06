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
	bool CleanUp();

	void SetWindowTitle(const char* title);
	uint GetScale() { return (uint)scale; }

	int		width;
	int		height;
	int		scale;

	bool fullscreen = false;
	bool borderless = false;
	bool resizable = false;
	bool fullscreen_window = false;

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

public:

	SDL_Window* window;
	SDL_Surface* screen_surface;

};

#endif // !WINDOW__H
