#include "UIingameUI.h"
#include "Modules/Gui.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/SceneController.h"
#include "UIhealthbar.h"
#include "EngineAPI.h"

UIingameui::UIingameui(Player * _player)
{
	player = _player;


}

void UIingameui::Init()
{
	hp = new UIhealthbar(HEALTH);//576
	hp->x = 0;
	hp->y = 528;
	hp->player = player;

	mana = new UIhealthbar(MANA);//576
	mana->x = 0;
	mana->y = 480;
	mana->player = player;

	spells = new UIspelldisplay();
	spells->x = 400;
	spells->y = 476;//w = 224 // x = 400
	spells->player = player;

	Engine->GetModule<UserInterface>().AddElement(hp);
	Engine->GetModule<UserInterface>().AddElement(mana);
	Engine->GetModule<UserInterface>().AddElement(spells);
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
