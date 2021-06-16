#ifndef UI_HEALTHBAR
#define UI_HEALTHBAR


#include "UIelement.h"
#include "Player.h"

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

	int size_in_image = 48;

};
#endif // !UI_HEALTHBAR
