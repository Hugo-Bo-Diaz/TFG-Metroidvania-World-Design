#include "UIPauseMenu.h"
#include "Application.h"
#include "Modules/Textures.h"
#include "Modules/Render.h"
#include "Modules/Input.h"
#include "Modules/ObjectManager.h"
#include "Modules/Gui.h"
#include "Modules/Camera.h"
#include "Modules/ProgressTracker.h"
#include "UIingameUI.h"
#include "Modules/SceneController.h"
#include "UISettingsMenu.h"
#include "../SceneProcessing.h"

#include "Modules/Audio.h"
UIPauseMenu::UIPauseMenu()
{
	resume_rect = {0,0,362,72};
	settings_rect = {0,72,362,72};
	exit_rect = {0,144,362,72};
	
	Engine->GetModule<ObjectManager>().PauseObjects();

	TexMenuBase = Engine->GetModule<Textures>().Load_Texture("Assets/UI/pause_menu_base.png");
	TexMenuOptions = Engine->GetModule<Textures>().Load_Texture("Assets/UI/pause_menu_options.png");
}


UIPauseMenu::~UIPauseMenu()
{
}

void UIPauseMenu::Loop()
{
	if(Engine->GetModule<Input>().GetInput(BUTTON_1) == BUTTON_DOWN && !MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{
		switch (current_option)
		{
		case PAUSE_RESUME:
			Engine->GetModule<ObjectManager>().UnPauseObjects();
			MetroidVaniaSceneProcessor::GetInstance().is_pause_menu_up = false;
			Engine->GetModule<UserInterface>().RemoveElement(this);
			Engine->GetModule<Audio>().PlaySFX(SFX_MENU_SELECT);
			break;
		case PAUSE_SETTINGS:
			//App->trk->AddSettingsMenu(2);
			Engine->GetModule<Audio>().PlaySFX(SFX_MENU_SELECT);
			break;
		case PAUSE_EXIT:
			Engine->GetModule<ObjectManager>().UnPauseObjects();
			MetroidVaniaSceneProcessor::GetInstance().is_pause_menu_up = false;
			MetroidVaniaSceneProcessor::GetInstance().should_go_to_main_menu = true;
			Engine->GetModule<UserInterface>().RemoveElement(this);
			Engine->GetModule<ProgressTracker>().SaveGame("save_file.xml");
			Engine->GetModule<Camera>().CoverScreen(1000, 300, 0, 0, 0);
			Engine->GetModule<Audio>().PlaySFX(SFX_MENU_SELECT);
			break;
		default:
			break;
		}
	}
		

	float joyx;
	float joyy;
	Engine->GetModule<Input>().GetJoystick(true, joyx, joyy);

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

	if(abs(amount_of_movement)>0.6 && !stop_inputs && !MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{
		CycleOption(amount_of_movement);
		prev_joy_y.clear();
		stop_inputs = true;
		Engine->GetModule<Audio>().PlaySFX(SFX_MENU_CHANGE);
	}


	if (abs(joyy)<0.2)
	{
		stop_inputs = false;
	}


}

void UIPauseMenu::Render()
{

	//render base
	Engine->GetModule<::Render>().Blit(TexMenuBase, x, y, nullptr, -2, RenderQueue::RENDER_UI);

	//render highlighted option
	switch (current_option)
	{
	case PAUSE_RESUME:
		Engine->GetModule<::Render>().Blit(TexMenuOptions, x+90, y+164, &resume_rect, -2, RenderQueue::RENDER_UI);
		break;
	case PAUSE_SETTINGS:
		Engine->GetModule<::Render>().Blit(TexMenuOptions, x + 90, y + 260, &settings_rect, -2, RenderQueue::RENDER_UI);
		break;
	case PAUSE_EXIT:
		Engine->GetModule<::Render>().Blit(TexMenuOptions, x + 90, y + 356, &exit_rect, -2, RenderQueue::RENDER_UI);
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
