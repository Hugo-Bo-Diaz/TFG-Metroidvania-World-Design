#include "UISettingsMenu.h"
#include "UIPauseMenu.h"
#include "Application.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Gui.h"
#include "UIingameUI.h"
#include "SceneController.h"
#include "Audio.h"
#include "Window.h"

UISettingsMenu::UISettingsMenu()
{
	volume_music_bar_rect = {0,0,224,18};
	volume_sfx_bar_rect = { 0,0,224,18 };
	blue_mid_rect = {0,18,224,34};
	blue_big_rect = {0,52,224,64};
	blue_small_rect = {0,116,224,34};
}


UISettingsMenu::~UISettingsMenu()
{
}

void UISettingsMenu::Loop()
{

	float joyx;
	float joyy;
	App->inp->GetJoystick(true, joyx, joyy);	

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
			App->aud->sfx_volume -= 5*factor;

			if (App->aud->sfx_volume > 100)
				App->aud->sfx_volume = 100;

			if (App->aud->sfx_volume < 0)
				App->aud->sfx_volume = 0;
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
			App->aud->music_volume -= 5 * factor;

			if (App->aud->music_volume > 100)
				App->aud->music_volume = 100;

			if (App->aud->music_volume < 0)
				App->aud->music_volume = 0;
		}
	}
		break;
	case SETTINGS_FULLSCREEN_TOGGLE:
	{
		if (App->inp->GetInput(BUTTON_1) == BUTTON_DOWN)
		{
			App->win->ToggleFullScreen();
		}
	}
		break;
	case SETTINGS_BACK:
	{
		if (App->inp->GetInput(BUTTON_1) == BUTTON_DOWN)
		{
			App->gui->RemoveElement(this);
			switch (parent_type)
			{
			case SETTINGS_PARENT_PAUSE_MENU:
			{
				((UIPauseMenu*)parent)->is_settings_up = false;
				((UIPauseMenu*)parent)->settings = nullptr;
			}
				break;
			default:
				break;
			}
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
		//printf("%f, ",*it);
	}
	last_joy_y = joyy;

	if (abs(amount_of_movement)>0.6 && !stop_inputs)
	{
		CycleOption(amount_of_movement);
		prev_joy_y.clear();
		stop_inputs = true;
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
	float volume_sfx = App->aud->sfx_volume;
	float volume_music= App->aud->music_volume;

	volume_music_bar_rect.w = 208 * (volume_music / 100);
	volume_sfx_bar_rect.w = 208 * (volume_sfx / 100);

	//render base
	App->ren->BlitUI(App->tex->Get_Texture("settings_menu_base"), x, y, nullptr, -2);
	//render bars
	App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"),x+14,y+146,&volume_music_bar_rect,-3);
	App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"), x+14, y+86, &volume_sfx_bar_rect, -3);

	//render highlighted option
	switch (current_option)
	{
	case SETTINGS_VOLUME_SFX:
		App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"), x + 6, y + 78, &blue_mid_rect, -2);
		break;
	case SETTINGS_VOLUME_MUSIC:
		App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"), x + 6, y + 138, &blue_mid_rect, -2);
		break;
	case SETTINGS_FULLSCREEN_TOGGLE:
		App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"), x + 6, y + 174, &blue_big_rect, -2);
		break;
	case SETTINGS_BACK:
		App->ren->BlitUI(App->tex->Get_Texture("settings_menu_options"), x + 6, y + 246, &blue_small_rect, -2);
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
