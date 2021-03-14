#include "Fire.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "FireBall.h"
#include "Render.h"
#include "Camera.h"

void Fire::Init()
{
	firebreath_left.AddFrame({ 224,0,32,64 });
	firebreath_right.AddFrame({ 224,64,32,64 });
	fireshield.AddFrame({ 0,64,96,96 });

	fireball_big = { 0,0,64,64 };
	fireball_small = { 96,0,32,32 };


	fireshield_timer.Pause();
	fireshield_timer.Reset();
}

void Fire::Loop(float dt)
{
	//Fireball--------------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_2) == KEY_RELEASE)
	{

		if (charge > 100)
		{
			FireBall* fireball = (FireBall*)App->phy->AddObject(player->collider->x, player->collider->y, 32, 32, FIRE_BALL);
			fireball->Fire(player->is_right, true);
			App->cam->CameraShake(20, 120);
			//printf("big fireball \n");
		}
		else
		{
			FireBall* fireball = (FireBall*)App->phy->AddObject(player->collider->x, player->collider->y, 32, 32, FIRE_BALL);
			fireball->Fire(player->is_right, false);
			//printf("small fireball \n");
			App->cam->CameraShake(3, 120);

		}
		charge = 0;
	}

	if (App->inp->GetInput(BUTTON_2) == KEY_REPEAT)
	{
		charge += 2;
		if (charge > 100)
		{
			if (player->is_right)
			{
				App->ren->Blit(App->tex->Get_Texture("spells"),
					player->collider->x + player->collider->w - 5,
					player->collider->y + player->collider->h / 2 - fireball_big.h / 2,
					&fireball_big, -2);
			}
			else
			{
				App->ren->Blit(App->tex->Get_Texture("spells"),
					player->collider->x - fireball_big.w + 5,
					player->collider->y + player->collider->h / 2 - fireball_big.h / 2,
					&fireball_big, -2);
			}
		}
		else
		{
			if (player->is_right)
			{
				App->ren->Blit(App->tex->Get_Texture("spells"),
					player->collider->x + player->collider->w - 15,
					player->collider->y + player->collider->h / 2 - fireball_small.h / 2,
					&fireball_small, -2);
			}
			else
			{
				App->ren->Blit(App->tex->Get_Texture("spells"),
					player->collider->x - fireball_small.w + 15,
					player->collider->y + player->collider->h / 2 - fireball_small.h / 2,
					&fireball_small, -2);
			}
		}
	}
	//Fireslash/breath------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && player->grounded)
	{
		player->LockMovement();
		firebreath_is_active = true;
	}

	if (firebreath_is_active)
	{
		if(player->is_right)
		{
			App->ren->Blit(App->tex->Get_Texture("spells"),
				player->collider->x + player->collider->w,
				player->collider->y,
				firebreath_right.GetCurrentFrame(), -2);
		}
		else
		{
			App->ren->Blit(App->tex->Get_Texture("spells"),
				player->collider->x - firebreath_left.GetCurrentFrame()->w,
				player->collider->y,
				firebreath_left.GetCurrentFrame(), -2);
		}

		//Get nearby objects and do effect

	}

	if (App->inp->GetInput(BUTTON_3) == KEY_RELEASE)
	{
		player->UnlockMovement();
		firebreath_is_active = false;
	}

	//Fireshield--------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && !is_fireshield_on_cooldown)
	{
		player->LockMovement(shield_activation_time);
		is_fireshield_on_cooldown = true;
		fireshield_timer.Start();
		fireshield_timer.Reset();
	}
	
	if (fireshield_timer.Read() > shield_activation_time && !is_fireshield_up)
	{
		is_fireshield_up = true;
		//draw and attack etc...
	}

	if (fireshield_timer.Read() > shield_max_time)
	{
		is_fireshield_up = false;
	}

	if (fireshield_timer.Read() > cooldown_shield)
	{
		is_fireshield_on_cooldown = false;
	}

	if (is_fireshield_up)
	{
		App->ren->Blit(App->tex->Get_Texture("spells"),
			player->collider->x + (player->collider->w - fireshield.GetCurrentFrame()->w)/2 ,
			player->collider->y + (player->collider->h - fireshield.GetCurrentFrame()->h) / 2,
			fireshield.GetCurrentFrame(), -2);
	}
}

void Fire::UnlockMovementEvent()
{

}


void Fire::CleanUp()
{

}