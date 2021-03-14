#include "UIingameUI.h"
#include "Gui.h"

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
}

void UIingameui::Render()
{
	UIelement::Loop();
}
