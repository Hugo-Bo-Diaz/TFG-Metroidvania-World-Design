#include "Grass.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "../GameObjects/SpellProjectiles/Leaf.h"
#include "../GameObjects/SpellProjectiles/Thorns.h"
#include "Render.h"
#include "Camera.h"
#include "MathHelp.h"
#include "Particles.h"
#include "Utils.h"

Grass::~Grass()
{
	App->par->RemoveParticleEmitter(p);
	//App->par->to_delete.push_back(p);
}

void Grass::Init()
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

	thorns_timer.Pause();
	thorns_timer.Reset();
	p = App->par->AddParticleEmitter(&grass, 0, 0);
	p->active = false;
}

void Grass::Loop(float dt)
{
	
	//leafshot--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_RELEASE)
	p->position_x = player->collider->x + player->collider->w / 2;
	p->position_y = player->collider->y + player->collider->h / 2;


	if (App->inp->GetInput(BUTTON_2) == KEY_RELEASE)
	{
		if (charge > 25)
		{
			/*Leaf* leaf = (Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, LEAF);
			leaf->Fire(player->is_right, 15);*/
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, GetTypeIndex<Leaf>()))->Fire(player->is_right, 15);
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, GetTypeIndex<Leaf>()))->Fire(player->is_right, 0);
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, GetTypeIndex<Leaf>()))->Fire(player->is_right, -15);

			App->cam->CameraShake(20, 100);
		}
		else
		{
			Leaf* leaf = (Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, GetTypeIndex<Leaf>());
			leaf->Fire(player->is_right, 0);

			App->cam->CameraShake(10, 50);
		}
		charge = 0;
	}

	if (App->inp->GetInput(BUTTON_2) == KEY_REPEAT)
	{
		charge += 2;
	}
	if (charge > 25)
	{
		p->active = true;
	}
	else
	{
		p->active = false;
	}
	
	//grasshook------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN)
	{
		if (!hooked && !hook_out)
		{
			float x,y;
			App->inp->GetJoystick(true,x,y);
			current_angle = 3.1428 +(atan2(-y,-x));
			hook_position_x = player->x + player->collider->w / 2;
			hook_position_y = player->y + player->collider->h / 2;
			hook_out = true;
		}
		if (hooked)
		{
			//unhook
			player->UnlockMovement();
			hook_out = false;
			hooked = false;
		}
		

	}
	if (hook_out)
	{
		if (DistanceBetweenTwoPoints(player->x, player->y, hook_position_x, hook_position_y) >=1000)
		{
			hook_out = false;

			hook_position_x = player->x + player->collider->w / 2;
			hook_position_y = player->y + player->collider->h / 2;
		}

	
		hook_position_x += cos(current_angle)*speed_hook;
		hook_position_y += sin(current_angle)*speed_hook;


		//now we look for a wall within range
		std::vector<SDL_Rect*> colliders;
		App->phy->GetNearbyWalls(hook_position_x, hook_position_y, 100, colliders);
		for (int i = 0; i < colliders.size(); ++i)
		{
			//top line
			/*if (LineIntersection(player->x, player->y,
					second_point_x, second_point_y,
					colliders[i]->x, colliders[i]->y,
					colliders[i]->x + colliders[i]->w, colliders[i]->y,
					inter_x, inter_y)
					&&
					DistanceBetweenTwoPoints(player->x, player->y, inter_x, inter_y) < distance_to_intersection)
					{
					intersection_x = inter_x;
					intersection_y = inter_y;
					distance_to_intersection = DistanceBetweenTwoPoints(player->x, player->y, inter_x, inter_y);
					}
				*/

			SDL_Point p = { hook_position_x,hook_position_y };
			if (SDL_PointInRect(&p, colliders[i]) == SDL_TRUE)
			{
				hooked = true;
				hook_out = false;
				player->LockMovement();
				current_angle = atan2(hook_position_y - player->y, hook_position_x - player->x);
			}
		}
	}
	


	if (hooked)
	{
		if (DistanceBetweenTwoPoints(hook_position_x, hook_position_y, player->x + player->collider->w / 2, player->y + player->collider->h / 2) > 64)
		{
			current_angle = atan2(hook_position_y - player->y, hook_position_x - player->x);

			player->nextpos->x += cos(current_angle)*speed_player;
			player->nextpos->y += sin(current_angle)*speed_player;
		}
		
		bool stay_hooked = false;

		std::vector<SDL_Rect*> colliders;
		App->phy->GetNearbyWalls(hook_position_x, hook_position_y, 100, colliders);
		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Point p = { hook_position_x,hook_position_y };
			if (SDL_PointInRect(&p, colliders[i]) == SDL_TRUE)
			{
				stay_hooked = true;
			}
		}

		if (!stay_hooked)
		{
			player->UnlockMovement();
			hook_out = false;
			hooked = false;
		}

	}
	


	
	//thorns terrain--------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && !is_thorns_on_cooldown)
	{
		is_thorns_on_cooldown = true;
		Thorns* t = (Thorns*)App->phy->AddObject(player->x+player->collider->w/2, player->y + player->collider->h / 2,32,32, GetTypeIndex<Thorns>());
		t->Fire(player->is_right,thorns_max_time);
		thorns_timer.Reset();
		thorns_timer.Start();
	}

	if (is_thorns_on_cooldown && thorns_timer.Read() > cooldown_thorns)
	{
		is_thorns_on_cooldown = false;
	}
}

void Grass::Render()
{
	if (hooked || hook_out)
	{
		App->ren->Blit(spells, hook_position_x - hook.w / 2, hook_position_y - hook.h / 2, &hook, -2, RenderQueue::RENDER_GAME, 0);
		debug.x = hook_position_x - 5;
		debug.y = hook_position_y - 5;
		App->ren->DrawRect(debug, 0, 0, 0, 255, true, RenderQueue::RENDER_GAME,0);

		draw_angle = atan2(hook_position_y - (player->y + (player->collider->h / 2)), hook_position_x - (player->x + (player->collider->w / 2)));
		float distance_to_hook = DistanceBetweenTwoPoints(hook_position_x, hook_position_y, player->x + player->collider->w / 2, player->y + player->collider->h / 2);
		int vine_portions = 1 + (distance_to_hook / 32);
		for (int i = 0; i < vine_portions; ++i)
		{
			App->ren->Blit(spells,
				player->x + player->collider->w / 2 + cos(draw_angle)*i * 32 - vine.w / 2,
				player->y + player->collider->h / 2 + sin(draw_angle)*i * 32 - vine.h / 2,
				&vine, -1, RenderQueue::RENDER_GAME, draw_angle * 180 / 3.1428);
		}
	}
}

void Grass::Switched_in()
{
}

void Grass::Switched_out()
{
	player->UnlockMovement();
	hook_out = false;
	hooked = false;

	hook_position_x = player->x + player->collider->w / 2;
	hook_position_y = player->y + player->collider->h / 2;

	charge = 0;
}

void Grass::UnlockMovementEvent()
{

}


void Grass::CleanUp()
{

}