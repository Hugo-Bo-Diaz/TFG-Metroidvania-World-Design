#include "UISelectMenu.h"
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
#include "Text.h"
#include "../SceneProcessing.h"

UISelectMenu::UISelectMenu()
{
	//for (int i = 0; i < MAX_LORE_CAPACITY_MENU; ++i)
	//{
	//	different_texts[i] = App->txt->CreateText("", SDL_Color{ 0,0,0,0 }, 500,"",1);
	//}

	//description = App->txt->CreateText("", SDL_Color{ 0,0,0,0 }, 335, "", 2);

	//selectquad = {0,0,206,28 };

	App->phy->PauseObjects();

	selectMenuBack = App->tex->Load_Texture("Assets/UI/selectmenubackground.png");
	selectLogBook = App->tex->Load_Texture("Assets/UI/selectmenulogbook.png");
	selectOptions = App->tex->Load_Texture("Assets/UI/selectmenuoptions.png");

}


UISelectMenu::~UISelectMenu()
{
	//delete all the things :)
}

void UISelectMenu::Loop()
{

	switch (current_option)
	{
	case SELECT_MENU_LORE:
	{
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

		if (abs(amount_of_movement) > 0.6 && !stop_inputs && App->trk->BaseSaveSection->GetChild("Logs")->GetSectionValues().size() > 1)
		{
			if (amount_of_movement > 0 && current_lore < App->trk->BaseSaveSection->GetChild("Logs")->GetSectionValues().size() -1)
			{
				current_lore += 1;
				if (current_lore-cam_y_coord > MAX_LORE_CAPACITY_MENU-1)
				{
					//CycleText(amount_of_movement);
					cam_y_coord += 1;
				}
			}
			else if (amount_of_movement < 0 && current_lore > 0)
			{
				current_lore -= 1;
				if (current_lore < cam_y_coord)
				{
					//CycleText(amount_of_movement);
					cam_y_coord -= 1;
				}
			}

			prev_joy_y.clear();
			stop_inputs = true;
		}


		if (abs(joyy) < 0.2)
		{
			stop_inputs = false;
		}
	}
		break;
	case SELECT_MENU_MAP:
		break;
	default:
		break;
	}

	if ((App->inp->GetInput(SELECT) == BUTTON_DOWN || App->inp->GetInput(START) == BUTTON_DOWN) && !first_update)
	{

		App->phy->UnPauseObjects();
		MetroidVaniaSceneProcessor::GetInstance().is_pause_menu_up = false;
		App->gui->RemoveElement(this);
	}

	if (first_update)
	{
		first_update = false;
	}

}

void UISelectMenu::Render()
{

	//render base
	App->ren->Blit(selectMenuBack, x, y, nullptr, -2, RenderQueue::RENDER_UI);

	//render highlighted option
	//switch (current_option)
	//{
	//case SELECT_MENU_LORE:
	//{
	//	App->ren->BlitUI(selectLogBook, x, y, NULL, -3);
	//	//print all the available logs

	//	//for (int i = cam_y_coord; i < MAX_LORE_CAPACITY_MENU +cam_y_coord; ++i)
	//	if (App->trk->BaseSaveSection->GetChild("Logs")->GetSectionValues().size() > 0)
	//	{
	//		for (int i = cam_y_coord; i < MAX_LORE_CAPACITY_MENU+cam_y_coord && i < App->trk->BaseSaveSection->GetChild("Logs")->GetSectionValues().size(); ++i)
	//		{
	//			//LoreLog* l = App->trk->GetLog(i);
	//			//different_texts[i - cam_y_coord]->text = App->trk->active_logs[i]->title;
	//			//different_texts[i - cam_y_coord]->current_letter = different_texts[i - cam_y_coord]->text.size();
	//			std::string temp = MetroidVaniaSceneProcessor::GetInstance().GetLog(i)->title;

	//			different_texts[i - cam_y_coord]->ChangeText(temp.c_str() );

	//			App->ren->BlitText(different_texts[i - cam_y_coord],
	//				x + 110,
	//				y + 118 + (i - cam_y_coord) * 28 + 
	//				different_texts[i - cam_y_coord]->font_used->hsize * different_texts[i - cam_y_coord]->scale);

	//			if (current_lore == i)
	//			{
	//				int new_x = x + 110;
	//				int new_y = y + 114 + (i - cam_y_coord) * 28;
	//				//App->ren->DrawRect(&selectquad, 0, 0, 0, 150, true);
	//				App->ren->BlitUI(selectOptions, new_x, new_y, &selectquad, -4);
	//			}

	//		}
	//	
	//		//description->text = App->trk->active_logs[current_lore]->text;
	//		//description->current_letter = description->text.size();

	//		description->ChangeText(MetroidVaniaSceneProcessor::GetInstance().GetLog(current_lore)->text.c_str());

	//		//print the log itself
	//		App->ren->BlitText(description, x + 320, y + 114 + description->font_used->hsize*description->scale + 15);
	//	}
	//}
	//	break;
	//case SELECT_MENU_MAP:
	//	break;
	//default:
	//	break;
	//}
}

void UISelectMenu::CycleOption(float direction)
{
	/*
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
	}*/
}

void UISelectMenu::UpdateTexts()
{



}

void UISelectMenu::CycleText(float direction)
{
	if (direction > 0)//move up everything
	{

		//iterate 0 to 9

		for (int i = 0; i < MAX_LORE_CAPACITY_MENU-1; ++i)
		{
			//different_texts[i]->text = different_texts[i + 1]->text;

		}

		//different_texts[MAX_LORE_CAPACITY_MENU - 1]->text = App->trk-;
	}
	else//move everything down
	{
		//reverse iterate from 9 to 0
		for (int i = MAX_LORE_CAPACITY_MENU - 1; i >= 0; --i)
		{
		}



	}
}
