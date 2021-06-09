#include "HazardsRockBlock.h"
#include "Application.h"
#include "Render.h"

#include "Player.h"
#include "Ground.h"
#include "Particles.h"

#include "ProgressTracker.h"
#include "Textures.h"

HazardRockBlock::HazardRockBlock()
{
	rockblock = { 48,48,48,48 };
}

HazardRockBlock::~HazardRockBlock()
{
	App->phy->DeleteWall(wall_id);
}

void HazardRockBlock::Init()
{
	//wall_id = App->phy->AddWall(SDL_Rect{ collider->x + 1,collider->y + 1,46,46 });

	wall_id = App->phy->AddWall(SDL_Rect{ collider->x,collider->y,48,48 });
	collider->y -= 5;
	collider->h += 5;
	
	//wall_id = App->phy->AddWall(*collider);
}

bool HazardRockBlock::Loop(float dt)
{
	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

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
						App->phy->DeleteObject(this);
						App->phy->DeleteWall(wall_id);
						App->par->AddParticleEmitter(&App->par->rockblockexplosion, collider->x + collider->w / 2, collider->y + collider->h / 2, 300);
					}
				}
			}

		}
	}

	return true;
}

bool HazardRockBlock::Render()
{

	App->ren->Blit(App->tex->Get_Texture("hazards"), collider->x, collider->y+5, &rockblock, -20);
	

	return true;
}
