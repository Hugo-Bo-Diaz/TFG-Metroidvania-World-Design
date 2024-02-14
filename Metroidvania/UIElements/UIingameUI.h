#ifndef UI_INGAME
#define UI_INGAME

#include "UIelement.h"
#include "../GameObjects/Player.h"
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
	
	void SetPlayer(Player* player);
};

#endif // !UI_INGAME