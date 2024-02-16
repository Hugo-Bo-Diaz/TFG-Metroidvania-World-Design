#include "IceBlock.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"

IceBlock::IceBlock()
{
	spells = App->tex->Load_Texture("Assets/Sprites/spells.png");

	SDL_Rect wall = {collider->x,collider->y,collider->w,collider->h};
}

void IceBlock::Init()
{
	wall_id = App->phy->AddWall(*collider);
}

bool IceBlock::Loop(float dt)
{
	bool ret = true;

	if(timer.Read()>life_expectancy)
	{
		App->phy->DeleteWall(wall_id);
		App->phy->DeleteObject(this);
	}



	return ret;
}

bool IceBlock::Render()
{		App->ren->Blit(spells, collider->x, collider->y, &spr_on_img, -2);

	return true;
}

//IceBlock::~IceBlock()
//{
//	//App->phy->DeleteWall(wall_id);
//
//	physobj::~physobj();
//}