#include "UImainmenu.h"
#include "Application.h"
#include "Input.h"
#include "ProgressTracker.h"
#include "Gui.h"
#include "Render.h"
#include "Textures.h"
#include "UISettingsMenu.h"

UIMainMenu::UIMainMenu()
{
}

void UIMainMenu::Loop()
{

	if (App->inp->GetInput(BUTTON_1) == BUTTON_DOWN && !is_settings_up)
	{
		switch (current_option)
		{
		case MAINMENU_NEWGAME:
			App->trk->start_new_game = true;
			App->gui->RemoveElement(this);
			break;
		case MAINMENU_LOADGAME:
			App->trk->start_load_game = true;
			App->gui->RemoveElement(this);
			break;
		case MAINMENU_SETTINGS:
			settings = (UISettingsMenu*)App->gui->AddSettingsMenu();
			settings->parent = this;
			settings->parent_type = SETTINGS_PARENT_PAUSE_MENU;
			settings->x += 76;
			settings->y += 92;
			is_settings_up = true;
			break;
		case MAINMENU_CREDITS:
			//design more UI :)
			break;
		case MAINMENU_EXIT:
			//quit the game
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
		//printf("%f, ",*it);
	}
	last_joy_y = joyy;
	printf("\n");

	if (abs(amount_of_movement)>0.6 && !stop_inputs && !is_settings_up)
	{
		CycleOption(amount_of_movement);
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
	App->ren->BlitUI(App->tex->Get_Texture("mainmenu"),0,0,NULL,0);

	App->ren->BlitUI(App->tex->Get_Texture("mainmenuselected"), offset_option_x, offset_option_y+current_option*interval_y, NULL, 0);


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
