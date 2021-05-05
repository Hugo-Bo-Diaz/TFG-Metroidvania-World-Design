#include "Grass.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Leaf.h"
#include "Thorns.h"
#include "Render.h"
#include "Camera.h"
#include "Math.h"
#include "Particles.h"

Grass::~Grass()
{
	App->par->RemoveParticleEmitter(p);
	//App->par->to_delete.push_back(p);
}

void Grass::Init()
{
	thorns_timer.Pause();
	thorns_timer.Reset();
	p = App->par->AddParticleEmitter(&App->par->grass, 0, 0);
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
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, LEAF))->Fire(player->is_right, 15);
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, LEAF))->Fire(player->is_right, 0);
			((Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, LEAF))->Fire(player->is_right, -15);

			App->cam->CameraShake(20, 100);
			//printf("big hadoken \n");
		}
		else
		{
			Leaf* leaf = (Leaf*)App->phy->AddObject(player->collider->x, player->collider->y, 48, 48, LEAF);
			leaf->Fire(player->is_right, 0);

			App->cam->CameraShake(10, 50);
			//printf("smol hadoken \n");
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
			printf("%f\n",current_angle);
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
					printf("%f\n", distance_to_intersection);
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
		Thorns* t = (Thorns*)App->phy->AddObject(player->x+player->collider->w/2, player->y + player->collider->h / 2,32,32,THORNS);
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
		App->ren->Blit(App->tex->Get_Texture("spells"), hook_position_x - hook.w / 2, hook_position_y - hook.h / 2, &hook, -2, 0);
		debug.x = hook_position_x - 5;
		debug.y = hook_position_y - 5;
		App->ren->DrawRect(&debug, 0, 0, 0, 255, true);

		draw_angle = atan2(hook_position_y - (player->y + (player->collider->h / 2)), hook_position_x - (player->x + (player->collider->w / 2)));
		printf("%f\n", draw_angle * 180 / 3.1428);
		float distance_to_hook = DistanceBetweenTwoPoints(hook_position_x, hook_position_y, player->x + player->collider->w / 2, player->y + player->collider->h / 2);
		int vine_portions = 1 + (distance_to_hook / 32);
		for (int i = 0; i < vine_portions; ++i)
		{
			App->ren->Blit(App->tex->Get_Texture("spells"),
				player->x + player->collider->w / 2 + cos(draw_angle)*i * 32 - vine.w / 2,
				player->y + player->collider->h / 2 + sin(draw_angle)*i * 32 - vine.h / 2,
				&vine, -1, draw_angle * 180 / 3.1428);
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