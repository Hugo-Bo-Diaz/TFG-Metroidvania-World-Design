#ifndef UI_SELECT_MENU
#define UI_SELECT_MENU

#include "UIelement.h"
#include <list>
#include "Textures.h"

#define MAX_LORE_CAPACITY_MENU 10

struct TextPrint;

enum SelectMenuScreen {
	SELECT_MENU_LORE,
	SELECT_MENU_MAP,
};

class UISelectMenu : public UIelement
{
public:
	UISelectMenu();
	~UISelectMenu();

	SelectMenuScreen current_option = SELECT_MENU_LORE;

	void Loop();
	void Render();

	std::list<float> prev_joy_y;
	float last_joy_y = 0;
	bool stop_inputs = false;

	//LORE SCREEN
	int cam_y_coord = 0;
	int current_lore = 0;

	void CycleOption(float direction);

	TextPrint* different_texts[MAX_LORE_CAPACITY_MENU];
	TextPrint* description;

	SDL_Rect selectquad;

	void UpdateTexts();
	void CycleText(float direction);

	bool first_update = true;

	TextureID selectMenuBack;
	TextureID selectLogBook;
	TextureID selectOptions;
};

#endif