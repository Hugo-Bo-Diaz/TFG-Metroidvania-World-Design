#ifndef UI_MAIN_MENU
#define UI_MAIN_MENU

#include "EngineElements/ParticleEmitter.h"
#include "../GameObjects/Player.h"
#include "Modules/Textures.h"
#include "EngineElements/UIelement.h"
#include "Modules/Audio.h"

class UISettingsMenu;

enum MainMenuOption {
	MAINMENU_NEWGAME,
	MAINMENU_LOADGAME,
	MAINMENU_SETTINGS,
	MAINMENU_CREDITS,
	MAINMENU_EXIT
};

class UIMainMenu : public UIelement
{
public:

	UIMainMenu();

	std::list<float> prev_joy_y;
	float last_joy_y = 0;
	bool stop_inputs = false;

	void CycleOption(float direction);

	UISettingsMenu* settings;

	MainMenuOption current_option;

	void Init();
	void Loop();
	void Render();

	bool first_update = true;

	int offset_option_x = 500;
	int offset_option_y = 232;
	int interval_y = 68;

	TextureID TexBase;
	TextureID TexSelected;
	TextureID TexDisabled;

	AudioID mMenuMusic;
	AudioID mSFXMenuSelect;
	AudioID mSFXPing;
	AudioID mSFXMenuChange;
};
#endif // !UI_HEALTHBAR
