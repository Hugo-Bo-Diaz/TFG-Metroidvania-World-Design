#ifndef UI_MAIN_MENU
#define UI_MAIN_MENU


#include "UIelement.h"
#include "Player.h"


class UIMainMenu : public UIelement
{
public:

	UIMainMenu();

	void Loop();
	void Render();
};
#endif // !UI_HEALTHBAR
