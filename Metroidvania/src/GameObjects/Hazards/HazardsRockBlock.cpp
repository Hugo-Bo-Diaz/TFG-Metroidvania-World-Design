#include "HazardsRockBlock.h"
#include "Application.h"
#include "Modules/Render.h"

#include "../Player.h"
#include "../../Spells/Ground.h"
#include "Modules/Particles.h"

#include "Modules/ProgressTracker.h"

HazardRockBlock::HazardRockBlock()
{

	rockblock = { 48,48,48,48 };
}

void HazardRockBlock::Destroy()
{
	Engine->GetModule<ObjectManager>().DeleteWall(wall_id);
}

void HazardRockBlock::Init()
{
	//wall_id = Engine->GetModule<ObjectManager>().AddWall(RXRect{ collider->x + 1,collider->y + 1,46,46 });

	hazards = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/hazards.png");
	//wall_id = Engine->GetModule<ObjectManager>().AddWall(*collider);
}

bool HazardRockBlock::Loop(float dt)
{
	if (wall_id == -1)
	{
		RXRect lRect = { collider.x,collider.y,48,48 };
		wall_id = Engine->GetModule<ObjectManager>().AddWall(lRect);
		collider.y -= 5;
		collider.h += 5;
	}

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(&collider, collisions);
	/*
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{

			if ((*it)->type == PLAYER)
			{
				Player* p = (Player*)(*it)->object;
				if (p->current_spell == GROUND)
				{
					Ground* g = (Ground*)p->spells[GROUND];
					if (g->groundpounding)
					{
						//delete this object :^)
						Engine->GetModule<ObjectManager>().DeleteObject(this);
						Engine->GetModule<ObjectManager>().DeleteWall(wall_id);
						Engine->GetModule<Particles>().AddParticleEmitter(&Engine->GetModule<Particles>().rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
					}
				}
			}

		}
	}
	*/
	Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

	return true;
}

bool HazardRockBlock::Render()
{

	Engine->GetModule<::Render>().RenderTexture(hazards, collider.x, collider.y+5, rockblock, -20);
	

	return true;
}
