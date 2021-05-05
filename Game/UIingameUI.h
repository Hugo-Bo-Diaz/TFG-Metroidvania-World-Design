#ifndef UI_INGAME
#define UI_INGAME

#include "UIelement.h"
#include "Player.h"
#include "UIhealthbar.h"
#include "UIspelldisplay.h"

class UIingameui : public UIelement
{
public:

	UIingameui(Player* player);

	void Loop();
	void Render();

	Player* player;

	UIhealthbar* hp;
	UIhealthbar* mana;

	UIspelldisplay* spells;
	
};

#endif // !UI_INGAME
