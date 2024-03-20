#ifndef WINDOW__H
#define WINDOW__H

#include"PartsDef.h"
#include "Part.h"
#include <Windows.h>

//module that manages the window
class DLL_EXPORT Window : public Part
{
public:
	Window(EngineAPI& aAPI);

	//sets the window title
	void SetWindowTitle(const char* title);
	//returns the scale of the window
	float GetScale();
	//toggles the full screen feature
	void ToggleFullScreen();
	//returns the HWND window's handle for use on Graphic interfaces
	HWND GetSDLWindowHandle();
	//returns the window's size
	void GetWindowSize(int& x, int& y);

	class WindowImpl;
};

#endif // !WINDOW__H
