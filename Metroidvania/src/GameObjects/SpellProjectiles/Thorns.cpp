#include "Thorns.h"
#include "Application.h"
#include "Modules/Particles.h"
#include "Modules/Camera.h"

Thorns::Thorns()
{
}

void Thorns::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

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

	p = Engine->GetModule<Particles>().AddParticleEmitter(&grass, collider->x, collider->y);
	state = SEED;
}

bool Thorns::Loop(float dt)
{
	bool ret = true;
	switch (state)
	{
	case SEED:
	{
		collider->x += speed;

		collider->y += y_speed;

		y_speed += gravity;

		p->position_x = collider->x + collider->w / 2;
		p->position_y = collider->y + collider->h / 2;


		std::vector<SDL_Rect*> colliders;
		Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Rect result;
			if (SDL_IntersectRect(colliders[i], collider, &result) == SDL_TRUE)// he goin crash!
			{
				state = THORNS_ON_FLOOR;
				int tile_x = collider->x / 32;
				int tile_y = collider->y / 32;
				collider->x = tile_x * 32;
				collider->y = tile_y * 32;
				collider->w = 8;
				collider->h = 32;

				life_timer.Reset();
				life_timer.Start();

				Engine->GetModule<Camera>().CameraShake(25, 100);
			}
		}
	}
		break;
	case THORNS_ON_FLOOR:
	{

//		Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);


		std::vector<SDL_Rect*> colliders;

		if (direction == -1)
		{
			Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x, collider->y + collider->h / 2, 50, colliders);
			p->position_x = collider->x + collider->w;
		}
		else
		{
			Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w, collider->y + collider->h / 2, 50, colliders);
			p->position_x = collider->x;
		}

		for (int i = 0; i < colliders.size(); ++i)
		{
			if (direction == -1)
			{
				SDL_Point p = { collider->x + collider->w + 4, collider->y + collider->h + 8 };

				if (SDL_PointInRect(&p, colliders[i]) == SDL_FALSE)
				{
					expand = false;
				}
				SDL_Point p_f = { collider->x + collider->w + 4, collider->y +8};

				if (SDL_PointInRect(&p_f, colliders[i]) == SDL_TRUE)
				{
					expand = false;
				}
			}
			else
			{
				SDL_Point p = { collider->x - 4, collider->y + collider->h + 8 };

				if (SDL_PointInRect(&p, colliders[i]) == SDL_FALSE)
				{
					expand = false;
				}
				SDL_Point p_f = { collider->x - 4, collider->y + 8};

				if (SDL_PointInRect(&p_f, colliders[i]) == SDL_TRUE)
				{
					expand = false;
				}

			}
		}

		if (expand)
		{
			if (direction == -1)
			{
				collider->w += expansion_speed;
			}
			else
			{
					collider->w += expansion_speed;
					collider->x -= expansion_speed;
			}


		}


		

		if (life_timer.Read() > lifespan)
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
			//Engine->GetModule<Particles>().to_delete.push_back(p);
			Engine->GetModule<Particles>().RemoveParticleEmitter(p);
		}

	}
		break;
	}

	return ret;
}

bool Thorns::Render()
{

	switch (state)
	{
	case SEED:
	{

		Engine->GetModule<::Render>().Blit(spells, collider->x, collider->y, &seed, -1);
	}
	break;
	case THORNS_ON_FLOOR:
	{
		int tiles = collider->w / 32;
		float extra_distance = collider->w % 32;
		int i;
		for (i = 0; i< tiles; ++i)
		{
			if (i % 2 == 0)
			{
				//draw thorns 1
				Engine->GetModule<::Render>().Blit(spells, collider->x + i * 32, collider->y, &thorn_first, -2);
			}
			else
			{
				//draw thorns 2
				Engine->GetModule<::Render>().Blit(spells, collider->x + i * 32, collider->y, &thorn_second, -2);
			}
		}
		last_thorn_segment.w = extra_distance;
		Engine->GetModule<::Render>().Blit(spells, collider->x + i * 32, collider->y, &last_thorn_segment, -2);

	}
	}

	return true;
}


void Thorns::Fire(bool right_dir,float lifespan)
{
	//angle = _angle;

	if (!right_dir)
	{
		speed = -speed;
		direction = 1;
	}
	else
	{
		direction = -1;
	}
}