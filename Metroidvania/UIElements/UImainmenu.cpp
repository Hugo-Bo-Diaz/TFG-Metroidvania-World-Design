#include "UImainmenu.h"
#include "Application.h"
#include "Input.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "UISettingsMenu.h"
#include "../SceneProcessing.h"

UIMainMenu::UIMainMenu()
{
	TexBase = App->tex->Load_Texture("Assets/UI/mainmenu.png");
	TexSelected = App->tex->Load_Texture("Assets/UI/mainmenuselected.png");
	TexDisabled = App->tex->Load_Texture("Assets/UI/mainmenudisabled.png");
}

void UIMainMenu::Loop()
{

	if (first_update)
	{
		first_update = false;
		App->aud->PlayMusic(MUSIC_MENU, 500);
	}

	if (App->inp->GetInput(BUTTON_1) == BUTTON_DOWN && ! MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{

		switch (current_option)
		{
		case MAINMENU_NEWGAME:
			MetroidVaniaSceneProcessor::GetInstance().should_start_game = true;
			App->gui->RemoveElement(this);
			App->aud->PlaySFX(SFX_MENU_SELECT);
			break;
		case MAINMENU_LOADGAME:
			if (App->trk->CanLoadGame("save_file.xml"))
			{
				MetroidVaniaSceneProcessor::GetInstance().start_load_game = true;
				App->gui->RemoveElement(this);
				App->aud->PlaySFX(SFX_MENU_SELECT);
			}
			else
			{
				App->aud->PlaySFX(SFX_ENEMY_PING);
			}
			break;
		case MAINMENU_SETTINGS:
			//MetroidVaniaSceneProcessor::GetInstance().AddSettingsMenu(1);
			App->aud->PlaySFX(SFX_MENU_SELECT);
			break;
		case MAINMENU_CREDITS:
			//design more UI :)
			App->aud->PlaySFX(SFX_ENEMY_PING);
			break;
		case MAINMENU_EXIT:
			//quit the game
			MetroidVaniaSceneProcessor::GetInstance().should_exit = true;
			break;
		}
	}

	float joyx;
	float joyy;
	App->inp->GetJoystick(true, joyx, joyy);

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

	if (abs(amount_of_movement)>0.6 && !stop_inputs && !MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{
		CycleOption(amount_of_movement);
		App->aud->PlaySFX(SFX_MENU_CHANGE);
		prev_joy_y.clear();
		stop_inputs = true;
	}


	if (abs(joyy)<0.2)
	{
		stop_inputs = false;
	}


}

void UIMainMenu::Render()
{
	App->ren->Blit(TexBase,0,0,NULL,0, RenderQueue::RENDER_UI);

	App->ren->Blit(TexSelected, offset_option_x, offset_option_y+current_option*interval_y, NULL, 0, RenderQueue::RENDER_UI);


	if (!App->trk->CanLoadGame("save_file.xml"))
	{
		App->ren->Blit(TexDisabled,516,312,NULL,-2, RenderQueue::RENDER_UI);
	}

}


void UIMainMenu::CycleOption(float direction)
{

	if (direction > 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == MAINMENU_EXIT)
			new_option = MAINMENU_EXIT;
		else
			new_option += 1;


		current_option = (MainMenuOption)new_option;
	}
	else if (direction < 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == MAINMENU_NEWGAME)
			new_option = MAINMENU_NEWGAME;
		else
			new_option -= 1;

		current_option = (MainMenuOption)new_option;
	}

}
