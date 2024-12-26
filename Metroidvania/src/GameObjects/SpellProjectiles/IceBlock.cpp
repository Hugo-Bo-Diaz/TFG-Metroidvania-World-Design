#include "IceBlock.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"

IceBlock::IceBlock()
{

	RXRect wall = {collider.x,collider.y,collider.w,collider.h};
}

void IceBlock::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/spells.png", spells);
	wall_id = Engine->GetModule<SceneController>().AddWall(collider);
}

bool IceBlock::Loop(float dt)
{
	bool ret = true;

	if(timer.Read()>life_expectancy)
	{
		Engine->GetModule<SceneController>().DeleteWall(wall_id);
		Engine->GetModule<SceneController>().DeleteObject(this);
	}



	return ret;
}

bool IceBlock::Render()
{		Engine->GetModule<::Render>().RenderTexture(spells, collider.x, collider.y, spr_on_img, -2);

	return true;
}

IceBlock::~IceBlock()
{
	Engine->GetModule<SceneController>().DeleteWall(wall_id);

	GameObject::~GameObject();
}