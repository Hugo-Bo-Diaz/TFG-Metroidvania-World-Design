#ifndef CHECKPOINT__INDICATOR__H
#define CHECKPOINT__INDICATOR__H

#include "UIelement.h"
#include "Text.h"
#include "Timer.h"
#include "Textures.h"

class UIcheckpointIndicator : public UIelement
{
public:
	UIcheckpointIndicator();

	FontID mFont;

	SDL_Rect on_screen;
	float text_x;
	float text_y;

	void Loop();
	void Render();

	float max_time_to_dissapear = 2000;
	Timer to_dissapear;

	~UIcheckpointIndicator();

	TextureID Texture;
};

#endif
