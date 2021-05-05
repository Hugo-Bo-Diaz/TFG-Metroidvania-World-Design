#include "UIingameUI.h"
#include "Gui.h"
#include "Application.h"
#include "Input.h"
#include "SceneController.h"

UIingameui::UIingameui(Player * _player)
{
	player = _player;

	hp = (UIhealthbar*)App->gui->Add_ResourceBar(0,528,HEALTH);//576
	hp->player = _player;

	mana = (UIhealthbar*)App->gui->Add_ResourceBar(0, 480,MANA);
	mana->player = _player;

	spells = (UIspelldisplay*)App->gui->AddSpellDisplay(400,476);//w = 224 // x = 400
	spells->player = _player;
}

void UIingameui::Loop()
{
	UIelement::Loop();

	if (App->inp->GetInput(START) == BUTTON_DOWN && !App->scn->is_pause_menu_up && !App->scn->is_select_menu_up)
	{
		App->gui->AddPauseMenu();
		App->scn->is_pause_menu_up = true;
	}


	if (App->inp->GetInput(SELECT) == BUTTON_DOWN && !App->scn->is_select_menu_up && !App->scn->is_pause_menu_up)
	{
		App->gui->AddSelectMenu();
		App->scn->is_pause_menu_up = true;
	}

}

void UIingameui::Render()
{
	UIelement::Loop();
}
