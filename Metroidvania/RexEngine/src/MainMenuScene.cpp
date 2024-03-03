#include "MainMenuScene.h"
#include "Application.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "SceneController.h"

bool SceneMainMenu::Create()
{
	 App->tex->Load_Texture("Assets/Backgrounds/MainMenuBack.png","backgroundbads");
	 App->tex->Load_Texture("Assets/Sprites/client_button.png","client");
	App->tex->Load_Texture("Assets/Sprites/server_button.png","server");
	return true;
};

bool SceneMainMenu::Start()
{
	return true;

};

bool SceneMainMenu::Loop()
{
	App->ren->Blit(background,0,0,NULL,10);
	switch (current)
	{
	case SERVER_OPTION:
		App->ren->Blit(tex_client_button, 384, 300, option_not_chosen, 0);
		App->ren->Blit(tex_server_button, 384, 500, option_chosen, 0);
		break;
	case CLIENT_OPTION:
		App->ren->Blit(tex_client_button, 384, 300, option_chosen, 0);
		App->ren->Blit(tex_server_button, 384, 500, option_not_chosen, 0);
		break;
	default:
		App->ren->Blit(tex_client_button, 384, 300, option_not_chosen, 0);
		App->ren->Blit(tex_server_button, 384, 500, option_not_chosen, 0);
		break;
	}

	if (int i = App->inp->TapJoystick(true, 1) != 0)
	{
		int current_i = current;
		
		current_i += i;

		if (current_i == MIN_OPTION)
		{
			current_i = MAX_OPTION - 1;
		}
		else if (current_i == MAX_OPTION)
		{
			current_i = MIN_OPTION + 1;
		}
		
		current = (MainMenuOption)current_i;
	}

	if (App->inp->GetButton(Y) == BUTTON_DOWN)
	{
		switch (current)
		{
		case SERVER_OPTION:
			break;
		case CLIENT_OPTION:
			break;
		default:
			break;
		}
		App->scn->ChangeScene(IN_GAME);

	}

	return true;

};

bool SceneMainMenu::Stop()
{
	current = MIN_OPTION;
	return true;

};

bool SceneMainMenu::Destroy()
{
	/*if (App->tex->Valid_Texture(background))
	{
		App->tex->Destroy_Texture(background);
	}

	if (App->tex->Valid_Texture(tex_client_button))
	{
		App->tex->Destroy_Texture(tex_client_button);
	}

	if (App->tex->Valid_Texture(tex_server_button))
	{
		App->tex->Destroy_Texture(tex_server_button);
	}*/
	return true;

};