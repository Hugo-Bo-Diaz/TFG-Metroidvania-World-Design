#ifndef UI_HEALTHBAR
#define UI_HEALTHBAR

#include "EngineElements/ParticleEmitter.h"
#include "../GameObjects/Player.h"
#include "Modules/Textures.h"
#include "EngineElements/UIelement.h"

enum playerresource
{
	HEALTH,
	MANA

};

class UIhealthbar : public UIelement
{
public:
	//image name = healthbars

	UIhealthbar(playerresource resource);

	Player * player;
	int max_res;
	int curr_res;

	//curr_res = number of bars to draw
	
	playerresource res;
	SDL_Rect begin;
	SDL_Rect middle;
	SDL_Rect end;
	SDL_Rect bar;

	SDL_Rect last_bar;

	void Loop();
	void Render();
	void RenderDebug();

	int size_in_image = 48;

	TextureID TextureHB;
};
#endif // !UI_HEALTHBAR
