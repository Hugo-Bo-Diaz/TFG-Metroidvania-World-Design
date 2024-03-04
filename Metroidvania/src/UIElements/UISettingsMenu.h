#ifndef UI_SETTINGS_MENU
#define UI_SETTINGS_MENU

#include "Modules/Textures.h"
#include "EngineElements/ParticleEmitter.h"
#include "Utils/Timer.h"
#include "EngineElements/UIelement.h"

enum SettingsParentType {
	SETTINGS_PARENT_MAIN_MENU,
	SETTINGS_PARENT_PAUSE_MENU
};

enum SettingsMenuOption {
	SETTINGS_VOLUME_SFX,
	SETTINGS_VOLUME_MUSIC,
	SETTINGS_FULLSCREEN_TOGGLE,
	SETTINGS_BACK
};

class UISettingsMenu : public UIelement
{
public:
	UISettingsMenu();
	~UISettingsMenu();

	SettingsMenuOption current_option = SETTINGS_VOLUME_SFX;

	SDL_Rect volume_music_bar_rect;
	SDL_Rect volume_sfx_bar_rect;
	SDL_Rect blue_mid_rect;
	SDL_Rect blue_big_rect;
	SDL_Rect blue_small_rect;

	std::list<float> prev_joy_y;
	float last_joy_y = 0;
	bool stop_inputs = false;

	void Init();
	void Loop();
	void Render();

	void CycleOption(float direction);

	bool is_volume_on_cooldown = false;
	Timer cooldown_timer;
	float cooldown = 100;

	SettingsParentType parent_type;

	TextureID lTextureBase;
	TextureID lTextureOptions;
};

#endif