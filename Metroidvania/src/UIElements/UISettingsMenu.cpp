#include "UISettingsMenu.h"
#include "UIPauseMenu.h"
#include "Application.h"
#include "Modules/Textures.h"
#include "Modules/Render.h"
#include "Modules/Input.h"
#include "Modules/Gui.h"
#include "UIingameUI.h"
#include "Modules/SceneController.h"
#include "Modules/Audio.h"
#include "Modules/Window.h"
#include "Modules/ProgressTracker.h"
#include "../SceneProcessing.h"

UISettingsMenu::UISettingsMenu()
{
	volume_music_bar_rect = {0,0,224,18};
	volume_sfx_bar_rect = { 0,0,224,18 };
	blue_mid_rect = {0,18,224,34};
	blue_big_rect = {0,52,224,64};
	blue_small_rect = {0,116,224,34};

}

void UISettingsMenu::Init()
{
	lTextureBase = Engine->GetModule<Textures>().Load_Texture("Assets/UI/settings_menu_base.png");
	lTextureOptions = Engine->GetModule<Textures>().Load_Texture("Assets/UI/settings_menu_options.png");

	mSFXMenuSelect = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/menu_choose2.wav");
	mSFXMenuChange = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/menu_change.wav");
}

UISettingsMenu::~UISettingsMenu()
{
}

void UISettingsMenu::Loop()
{

	float joyx;
	float joyy;
	Engine->GetModule<Input>().GetJoystick(true, joyx, joyy);

	switch (current_option)
	{
	case SETTINGS_VOLUME_SFX:
	{
		int factor=0;
		if (abs(joyx) > 0.3)
		{
			if (joyx < 0)
				factor = 1;
			if (joyx > 0)
				factor = -1;
		}

		if (!is_volume_on_cooldown && factor != 0)
		{
			is_volume_on_cooldown = true;
			cooldown_timer.Reset();
			Engine->GetModule<Audio>().sfx_volume -= 5*factor;

			Engine->GetModule<Audio>().PlaySFX(mSFXMenuChange);

			if (Engine->GetModule<Audio>().sfx_volume > 100)
				Engine->GetModule<Audio>().sfx_volume = 100;

			if (Engine->GetModule<Audio>().sfx_volume < 0)
				Engine->GetModule<Audio>().sfx_volume = 0;
		}
	}
		break;
	case SETTINGS_VOLUME_MUSIC:
	{
		int factor = 0;
		if (abs(joyx) > 0.3)
		{
			if (joyx < 0)
				factor = 1;
			if (joyx > 0)
				factor = -1;
		}

		if (!is_volume_on_cooldown && factor != 0)
		{
			is_volume_on_cooldown = true;
			cooldown_timer.Reset();
			Engine->GetModule<Audio>().music_volume -= 5 * factor;

			Engine->GetModule<Audio>().PlaySFX(mSFXMenuChange);

			if (Engine->GetModule<Audio>().music_volume > 100)
				Engine->GetModule<Audio>().music_volume = 100;

			if (Engine->GetModule<Audio>().music_volume < 0)
				Engine->GetModule<Audio>().music_volume = 0;
		}
	}
		break;
	case SETTINGS_FULLSCREEN_TOGGLE:
	{
		if (Engine->GetModule<Input>().GetInput(BUTTON_1) == BUTTON_DOWN)
		{
			Engine->GetModule<Window>().ToggleFullScreen();
			Engine->GetModule<Audio>().PlaySFX(mSFXMenuSelect);

		}
	}
		break;
	case SETTINGS_BACK:
	{
		if (Engine->GetModule<Input>().GetInput(BUTTON_1) == BUTTON_DOWN)
		{
			Engine->GetModule<UserInterface>().RemoveElement(this);
			MetroidVaniaSceneProcessor::GetInstance().is_settings_up = false;
			/*switch (parent_type)
			{
			case SETTINGS_PARENT_PAUSE_MENU:
			{
				((UIPauseMenu*)parent)->is_settings_up = false;
				((UIPauseMenu*)parent)->settings = nullptr;
			}
				break;
			default:
				break;
			}*/
			Engine->GetModule<Audio>().PlaySFX(mSFXMenuSelect);

		}
	}
		break;
	default:
		break;
	}


	if (!stop_inputs)
	{
		prev_joy_y.push_back(joyy - last_joy_y);
		if (prev_joy_y.size() > 5)
		{
			//remove front
			prev_joy_y.erase(prev_joy_y.begin());
		}
	}
	float amount_of_movement = 0;

	for (std::list<float>::iterator it = prev_joy_y.begin(); it != prev_joy_y.end(); it++)
	{
		amount_of_movement += *it;
	}
	last_joy_y = joyy;

	if (abs(amount_of_movement)>0.6 && !stop_inputs)
	{
		CycleOption(amount_of_movement);
		prev_joy_y.clear();
		stop_inputs = true;

		Engine->GetModule<Audio>().PlaySFX(mSFXMenuChange);
	}


	if (abs(joyy)<0.2)
	{
		stop_inputs = false;
	}


	if (cooldown_timer.Read() > cooldown)
	{
		is_volume_on_cooldown = false;
	}

}

void UISettingsMenu::Render()
{
	float volume_sfx = Engine->GetModule<Audio>().sfx_volume;
	float volume_music= Engine->GetModule<Audio>().music_volume;

	volume_music_bar_rect.w = 208 * (volume_music / 100);
	volume_sfx_bar_rect.w = 208 * (volume_sfx / 100);

	//render base
	Engine->GetModule<::Render>().Blit(lTextureBase,x, y, nullptr, -2, RenderQueue::RENDER_UI);
	//render bars
	Engine->GetModule<::Render>().Blit(lTextureOptions, x+14,y+146,&volume_music_bar_rect,-3, RenderQueue::RENDER_UI);
	Engine->GetModule<::Render>().Blit(lTextureOptions, x+14, y+86, &volume_sfx_bar_rect, -3, RenderQueue::RENDER_UI);

	//render highlighted option
	switch (current_option)
	{
	case SETTINGS_VOLUME_SFX:
		Engine->GetModule<::Render>().Blit(lTextureOptions, x + 6, y + 78, &blue_mid_rect, -2, RenderQueue::RENDER_UI);
		break;
	case SETTINGS_VOLUME_MUSIC:
		Engine->GetModule<::Render>().Blit(lTextureOptions, x + 6, y + 138, &blue_mid_rect, -2, RenderQueue::RENDER_UI);
		break;
	case SETTINGS_FULLSCREEN_TOGGLE:
		Engine->GetModule<::Render>().Blit(lTextureOptions, x + 6, y + 174, &blue_big_rect, -2, RenderQueue::RENDER_UI);
		break;
	case SETTINGS_BACK:
		Engine->GetModule<::Render>().Blit(lTextureOptions, x + 6, y + 246, &blue_small_rect, -2, RenderQueue::RENDER_UI);
		break;
	default:
		break;
	}
}

void UISettingsMenu::CycleOption(float direction)
{

	if (direction > 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == SETTINGS_VOLUME_SFX || current_option == SETTINGS_VOLUME_MUSIC || current_option == SETTINGS_FULLSCREEN_TOGGLE)
			new_option += 1;
		else if (current_option == SETTINGS_BACK)
			new_option = SETTINGS_VOLUME_SFX;

		current_option = (SettingsMenuOption)new_option;
	}
	else if (direction < 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == SETTINGS_VOLUME_MUSIC || current_option == SETTINGS_FULLSCREEN_TOGGLE || current_option == SETTINGS_BACK)
			new_option -= 1;
		else if (current_option == SETTINGS_VOLUME_SFX)
			new_option = SETTINGS_BACK;

		current_option = (SettingsMenuOption)new_option;
	}
}
