#include "WindSlash.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"

WindSlash::WindSlash()
{
	particles = App->tex->Load_Texture("Assets/Sprites/particles.png");
	spells = App->tex->Load_Texture("Assets/Sprites/spells.png");

	windslash.AddFrame({ 0,222,48,48 });
	windslash.AddFrame({ 48,222,48,48 });
	windslash.AddFrame({ 96,222,48,48 });
	windslash.AddFrame({ 144,222,48,48 });

	r5slash = { 0,12,12,12 };
	windslash_part.area_in_texture.push_back(&r5slash);
	windslash_part.name = "windslash";
	windslash_part.minmax_speed_y = std::make_pair(-0.1, 0.1);
	windslash_part.minmax_speed_x = std::make_pair(-0.1, 0.1);
	windslash_part.minmax_lifespan = std::make_pair(75, 150);
	windslash_part.minmax_angle_speed = std::make_pair(5, 15);
	windslash_part.minmax_angle = std::make_pair(0, 360);
	windslash_part.minmax_x_offset = std::make_pair(0, 48);
	windslash_part.minmax_y_offset = std::make_pair(0, 48);
	windslash_part.minmax_frequency = std::make_pair(5, 20);
	windslash_part.texture_name = particles;

	p = App->par->AddParticleEmitter(&windslash_part,collider->x,collider->h);
}

bool WindSlash::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;
	windslash.NextFrame();

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
			App->par->AddParticleEmitter(&windslash_part, collider->x, collider->y, 500);
		}
	}

	return ret;
}

bool WindSlash::Render()
{
	App->ren->Blit(spells, collider->x, collider->y, windslash.GetCurrentFrame(), -2);
	return true;
}

void WindSlash::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}