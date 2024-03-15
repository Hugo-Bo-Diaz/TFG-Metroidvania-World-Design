#include "UImainmenu.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Gui.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"
#include "UISettingsMenu.h"
#include "../SceneProcessing.h"

UIMainMenu::UIMainMenu()
{
}

void UIMainMenu::Init()
{
	TexBase = Engine->GetModule<Textures>().Load_Texture("Assets/UI/mainmenu.png");
	TexSelected = Engine->GetModule<Textures>().Load_Texture("Assets/UI/mainmenuselected.png");
	TexDisabled = Engine->GetModule<Textures>().Load_Texture("Assets/UI/mainmenudisabled.png");

	mMenuMusic = Engine->GetModule<Audio>().LoadSFX("Assets/Music/not-in-vain.ogg");
	mSFXMenuSelect = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/menu_choose2.wav");
	mSFXPing = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/ping.wav");
	mSFXMenuChange = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/menu_change.wav");
}

void UIMainMenu::Loop()
{

	if (first_update)
	{
		first_update = false;
		Engine->GetModule<Audio>().PlayMusic(mMenuMusic, 500);
	}

	if (Engine->GetModule<Input>().GetInput(BUTTON_1) == BUTTON_DOWN && ! MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{

		switch (current_option)
		{
		case MAINMENU_NEWGAME:
			MetroidVaniaSceneProcessor::GetInstance().should_start_game = true;
			Engine->GetModule<UserInterface>().RemoveElement(this);
			Engine->GetModule<Audio>().PlaySFX(mSFXMenuSelect);
			break;
		case MAINMENU_LOADGAME:
			if (Engine->GetModule<ProgressTracker>().CanLoadGame("save_file.xml"))
			{
				MetroidVaniaSceneProcessor::GetInstance().start_load_game = true;
				Engine->GetModule<UserInterface>().RemoveElement(this);
				Engine->GetModule<Audio>().PlaySFX(mSFXMenuSelect);
			}
			else
			{
				Engine->GetModule<Audio>().PlaySFX(mSFXPing);
			}
			break;
		case MAINMENU_SETTINGS:
			//MetroidVaniaSceneProcessor::GetInstance().AddSettingsMenu(1);
			Engine->GetModule<Audio>().PlaySFX(mSFXMenuSelect);
			break;
		case MAINMENU_CREDITS:
			//design more UI :)
			Engine->GetModule<Audio>().PlaySFX(mSFXPing);
			break;
		case MAINMENU_EXIT:
			//quit the game
			MetroidVaniaSceneProcessor::GetInstance().should_exit = true;
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

	if (abs(amount_of_movement)>0.6 && !stop_inputs && !MetroidVaniaSceneProcessor::GetInstance().is_settings_up)
	{
		CycleOption(amount_of_movement);
		Engine->GetModule<Audio>().PlaySFX(mSFXMenuChange);
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
	Engine->GetModule<::Render>().Blit(TexBase, 0, 0, {0,0,1024,576}, 0, RenderQueue::RENDER_UI);

	Engine->GetModule<::Render>().Blit(TexSelected, offset_option_x, offset_option_y + current_option * interval_y, {0,0,408,72}, 0, RenderQueue::RENDER_UI);


	if (!Engine->GetModule<ProgressTracker>().CanLoadGame("save_file.xml"))
	{
		Engine->GetModule<::Render>().Blit(TexDisabled, 516, 312, {0,0,368,48}, -2, RenderQueue::RENDER_UI);
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
