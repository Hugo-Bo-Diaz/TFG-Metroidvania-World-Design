#ifndef UI_PAUSE_MENU
#define UI_PAUSE_MENU

#include "EngineElements/UIelement.h"
#include <list>
#include "Modules/Textures.h"
#include "Modules/Audio.h"

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

	RXRect resume_rect;
	RXRect settings_rect;
	RXRect exit_rect;

	std::list<float> prev_joy_y;
	float last_joy_y = 0;
	bool stop_inputs = false;

	void Init();
	void Loop();
	void Render();

	void CycleOption(float direction);

	UISettingsMenu* settings;

	TextureID TexMenuBase;
	TextureID TexMenuOptions;

	AudioID mSFXMenuSelect;
	AudioID mSFXMenuChange;
};

#endif