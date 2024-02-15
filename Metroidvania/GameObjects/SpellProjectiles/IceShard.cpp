#include "IceShard.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"
IceShard::IceShard()
{
	ice_shard_right.AddFrame({ 162,134,48,24 });
	ice_shard_left.AddFrame({ 102,134,48,24 });

	r4ice = { 36,12,12,12 };
	ice.area_in_texture.push_back(&r4ice);
	ice.name = "ice";
	ice.minmax_speed_y = std::make_pair(-0.2, 0.2);
	ice.minmax_speed_x = std::make_pair(-0.4, 0.4);
	ice.minmax_lifespan = std::make_pair(75, 150);
	ice.minmax_x_offset = std::make_pair(22, 42);
	ice.minmax_y_offset = std::make_pair(4, 20);
	ice.minmax_frequency = std::make_pair(5, 20);
	ice.texture_name = "particles";

	p = App->par->AddParticleEmitter(&ice, collider->x, collider->y);
}

bool IceShard::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;

	p->position_x = collider->x;
	p->position_y = collider->y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			//App->par->to_delete.push_back(p);
			App->par->RemoveParticleEmitter(p);
			App->par->AddParticleEmitter(&ice, collider->x-8, collider->y, 300);
		}
	}

	return ret;
}

bool IceShard::Render()
{
	if (direction == 1)
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, ice_shard_right.GetCurrentFrame(), -2);
		ice_shard_right.NextFrame();
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, ice_shard_left.GetCurrentFrame(), -2);
		ice_shard_left.NextFrame();
	}

	return true;
}

void IceShard::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}