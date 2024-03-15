#ifndef WINDOW__H
#define WINDOW__H

#include"PartsDef.h"
#include "Part.h"
#include <Windows.h>

struct SDL_Window;
struct SDL_Surface;

class DLL_EXPORT Window : public Part
{
public:
	Window(EngineAPI& aAPI);
	void SetWindowTitle(const char* title);
	float GetScale();
	void ToggleFullScreen();
	HWND GetSDLWindowHandle();
	void GetWindowSize(int& x, int& y);

	class WindowImpl;
};

#endif // !WINDOW__H
