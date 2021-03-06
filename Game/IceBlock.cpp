#include "IceBlock.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"

IceBlock::IceBlock()
{
	SDL_Rect wall = {collider->x,collider->y,collider->w,collider->h};
}

void IceBlock::Init()
{
	wall_id = App->phy->AddWall(*collider);
}

bool IceBlock::Loop(float dt)
{
	bool ret = true;

	App->ren->Blit(App->tex->Get_Texture("iceblock"), collider->x, collider->y, nullptr, -2);

	if(timer.Read()>life_expectancy)
	{
		App->phy->DeleteWall(wall_id);
		App->phy->DeleteObject(this);
	}

	return ret;
}

IceBlock::~IceBlock()
{
	App->phy->DeleteWall(wall_id);
}