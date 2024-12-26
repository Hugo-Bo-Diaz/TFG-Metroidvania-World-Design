#ifndef CHECKPOINT__INDICATOR__H
#define CHECKPOINT__INDICATOR__H

#include "EngineElements/ParticleEmitter.h"
#include "Utils/Timer.h"
#include "Modules/Render.h"
#include "EngineElements/UIelement.h"

class UIcheckpointIndicator : public UIelement
{
public:
	UIcheckpointIndicator();

	RexFontID mFont;

	RXRect on_screen;
	float text_x;
	float text_y;

	void Init();
	void Loop();
	void Render();

	float max_time_to_dissapear = 2000;
	Timer to_dissapear;

	~UIcheckpointIndicator();

	RexTextureID Texture;

	std::string mText;

};

#endif
