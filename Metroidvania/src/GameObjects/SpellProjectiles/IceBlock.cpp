#include "IceBlock.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"

IceBlock::IceBlock()
{
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

	SDL_Rect wall = {collider->x,collider->y,collider->w,collider->h};
}

void IceBlock::Init()
{
	wall_id = Engine->GetModule<ObjectManager>().AddWall(*collider);
}

bool IceBlock::Loop(float dt)
{
	bool ret = true;

	if(timer.Read()>life_expectancy)
	{
		Engine->GetModule<ObjectManager>().DeleteWall(wall_id);
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}



	return ret;
}

bool IceBlock::Render()
{		Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, &spr_on_img, -2);

	return true;
}

IceBlock::~IceBlock()
{
	Engine->GetModule<ObjectManager>().DeleteWall(wall_id);

	GameObject::~GameObject();
}