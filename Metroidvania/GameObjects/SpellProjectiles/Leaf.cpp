#include "Leaf.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"

Leaf::Leaf()
{
	particles = App->tex->Load_Texture("Assets/Sprites/particles.png");
	spells = App->tex->Load_Texture("Assets/Sprites/spells.png");

	r10grass = { 24,24,12,12 };
	r11grass = { 36,24,12,12 };
	grass.area_in_texture.push_back(&r10grass);
	grass.area_in_texture.push_back(&r11grass);
	grass.name = "grass";
	grass.minmax_x_offset = std::make_pair(-20, 20);
	grass.minmax_y_offset = std::make_pair(-20, 20);
	grass.minmax_angle_speed = std::make_pair(5, 15);
	grass.minmax_angle = std::make_pair(0, 360);
	grass.minmax_scale_speed = std::make_pair(0.03, 0.04);
	grass.minmax_scale = std::make_pair(1, 1.2);
	grass.minmax_lifespan = std::make_pair(100, 200);
	grass.minmax_frequency = std::make_pair(5, 20);
	grass.texture_name = particles;

	leaf_right.AddFrame({ 96,64,64,32 });
	leaf_left.AddFrame({ 96,32,64,32 });//48 16
	p = App->par->AddParticleEmitter(&grass, collider->x, collider->y);
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
			//App->par->to_delete.push_back(p);
			App->par->RemoveParticleEmitter(p);
			App->par->AddParticleEmitter(&grass, collider->x + collider->w / 2, collider->y + collider->h / 2, 200);
		}
	}

	return ret;
}

bool Leaf::Render()
{
	if (direction == 1)
	{
		App->ren->Blit(spells, collider->x, collider->y, leaf_right.GetCurrentFrame(), -2, angle);
		leaf_right.NextFrame();
	}
	else
	{
		App->ren->Blit(spells, collider->x, collider->y, leaf_left.GetCurrentFrame(), -2, angle);
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