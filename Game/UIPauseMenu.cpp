#include "UIPauseMenu.h"
#include "Application.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Physics.h"
#include "Gui.h"
#include "Camera.h"
#include "ProgressTracker.h"
#include "UIingameUI.h"
#include "SceneController.h"
#include "UISettingsMenu.h"

UIPauseMenu::UIPauseMenu()
{
	resume_rect = {0,0,362,72};
	settings_rect = {0,72,362,72};
	exit_rect = {0,144,362,72};
	
	App->phy->PauseObjects();
}


UIPauseMenu::~UIPauseMenu()
{
}

void UIPauseMenu::Loop()
{
	if(App->inp->GetInput(BUTTON_1) == BUTTON_DOWN && !is_settings_up)
	{
		switch (current_option)
		{
		case PAUSE_RESUME:
			App->phy->UnPauseObjects();
			App->scn->is_pause_menu_up = false;
			App->gui->RemoveElement(this);
			break;
		case PAUSE_SETTINGS:
			settings = (UISettingsMenu*)App->gui->AddSettingsMenu();
			settings->parent = this;
			settings->parent_type = SETTINGS_PARENT_PAUSE_MENU;
			settings->x += 76;
			settings->y += 92;
			is_settings_up = true;
			break;
		case PAUSE_EXIT:
			App->phy->UnPauseObjects();
			App->scn->is_pause_menu_up = false;
			App->trk->go_to_main_menu = true;
			App->gui->RemoveElement(this);
			App->trk->SaveGame("save_file.xml");
			App->cam->CoverScreen(1000, 300, 0, 0, 0);
			break;
		default:
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

	if(abs(amount_of_movement)>0.6 && !stop_inputs && !is_settings_up)
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

void UIPauseMenu::Render()
{

	//render base
	App->ren->BlitUI(App->tex->Get_Texture("pause_menu_base"), x, y, nullptr, -2);

	//render highlighted option
	switch (current_option)
	{
	case PAUSE_RESUME:
		App->ren->BlitUI(App->tex->Get_Texture("pause_menu_options"), x+90, y+164, &resume_rect, -2);
		break;
	case PAUSE_SETTINGS:
		App->ren->BlitUI(App->tex->Get_Texture("pause_menu_options"), x + 90, y + 260, &settings_rect, -2);
		break;
	case PAUSE_EXIT:
		App->ren->BlitUI(App->tex->Get_Texture("pause_menu_options"), x + 90, y + 356, &exit_rect, -2);
		break;
	default:
		break;
	}
}

void UIPauseMenu::CycleOption(float direction)
{

	if (direction > 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == PAUSE_RESUME || current_option == PAUSE_SETTINGS)
			new_option += 1;
		else if (current_option == PAUSE_EXIT)
			new_option = PAUSE_RESUME;

		current_option = (PauseMenuOption)new_option;
	}
	else if (direction < 0)//cycle_down
	{
		int new_option = current_option;

		if (current_option == PAUSE_SETTINGS || current_option == PAUSE_EXIT)
			new_option -= 1;
		else if (current_option == PAUSE_RESUME)
			new_option = PAUSE_EXIT;

		current_option = (PauseMenuOption)new_option;
	}
}
