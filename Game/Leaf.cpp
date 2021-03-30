#include "Leaf.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Particles.h"

Leaf::Leaf()
{
	leaf_right.AddFrame({ 96,64,64,32 });
	leaf_left.AddFrame({ 96,32,64,32 });//48 16
	p = App->par->AddParticleEmitter(&App->par->grass, collider->x, collider->y);
}

bool Leaf::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed * ratio_x;
	nextpos->x += direction * speed * ratio_x;
	
	collider->y += direction * speed * ratio_y;
	nextpos->y += direction * speed * ratio_y;

	p->position_x = collider->x+collider->w/2;
	p->position_y = collider->y+collider->h/2;


	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			App->par->to_delete.push_back(p);
			App->par->AddParticleEmitter(&App->par->grass, collider->x + collider->w / 2, collider->y + collider->h / 2, 200);
		}
	}

	return ret;
}

bool Leaf::Render()
{
	if (direction == 1)
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, leaf_right.GetCurrentFrame(), -2, angle);
		leaf_right.NextFrame();
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, leaf_left.GetCurrentFrame(), -2, angle);
		leaf_left.NextFrame();
	}
	return true;
}

void Leaf::Fire(bool left_dir, float _angle)
{
	//angle = _angle;

	if (left_dir)
	{
		angle = -_angle;
		direction = 1;
	}
	else
	{
		angle = _angle;
		direction = -1;
	}

	ratio_x = cos(angle*(3.1428/180));
	ratio_y = sin(angle*(3.1428 / 180));
}