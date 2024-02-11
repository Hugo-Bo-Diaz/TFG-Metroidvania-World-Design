#include "UIingameUI.h"
#include "Gui.h"
#include "Application.h"
#include "Input.h"
#include "SceneController.h"
#include "UIhealthbar.h"

UIingameui::UIingameui(Player * _player)
{
	player = _player;

	hp = new UIhealthbar(HEALTH);//576
	hp->x = 0;
	hp->y = 528;
	hp->player = _player;

	mana = new UIhealthbar(MANA);//576
	mana->x = 0;
	mana->y = 480;
	mana->player = _player;

	spells = new UIspelldisplay();
	spells->x = 400;
	spells->y = 476;//w = 224 // x = 400
	spells->player = _player;

	App->gui->AddElement(hp);
	App->gui->AddElement(mana);
	App->gui->AddElement(spells);
}

void UIingameui::Loop()
{
	UIelement::Loop();


}

void UIingameui::Render()
{
	UIelement::Loop();
}

void UIingameui::SetPlayer(Player* player)
{
	hp->player = player;
	mana->player = player;
	spells->player = player;
}
