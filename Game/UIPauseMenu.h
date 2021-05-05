#ifndef UI_PAUSE_MENU
#define UI_PAUSE_MENU

#include "UIelement.h"
#include <list>

class UISettingsMenu;

enum PauseMenuOption {
	PAUSE_RESUME,
	PAUSE_SETTINGS,
	PAUSE_EXIT
};

class UIPauseMenu : public UIelement
{
public:
	UIPauseMenu();
	~UIPauseMenu();

	PauseMenuOption current_option =PAUSE_RESUME;

	SDL_Rect resume_rect;
	SDL_Rect settings_rect;
	SDL_Rect exit_rect;

	std::list<float> prev_joy_y;
	float last_joy_y = 0;
	bool stop_inputs = false;

	void Loop();
	void Render();

	void CycleOption(float direction);

	bool is_settings_up=false;
	UISettingsMenu* settings;
};

#endif