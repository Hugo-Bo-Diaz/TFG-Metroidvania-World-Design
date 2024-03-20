#include "IceBlock.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"

IceBlock::IceBlock()
{

	RXRect wall = {collider->x,collider->y,collider->w,collider->h};
}

void IceBlock::Init()
{
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");
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
{		Engine->GetModule<::Render>().RenderTexture(spells, collider->x, collider->y, spr_on_img, -2);

	return true;
}

IceBlock::~IceBlock()
{
	Engine->GetModule<ObjectManager>().DeleteWall(wall_id);

	GameObject::~GameObject();
}