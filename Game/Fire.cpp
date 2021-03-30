#include "Fire.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "FireBall.h"
#include "Render.h"
#include "Camera.h"
#include "Particles.h"
#include "LavaSpell.h"

void Fire::Init()
{
	firebreath_left.AddFrame({ 224,0,32,64 });
	firebreath_right.AddFrame({ 224,64,32,64 });
	fireshield.AddFrame({ 0,64,96,96 });

	fireball_big = { 0,0,64,64 };
	fireball_small = { 96,0,32,32 };

	to_follow = App->par->AddParticleEmitter(&App->par->fireshield,0,0);
	to_follow->active = false;

	volcano_particles = App->par->AddParticleEmitter(&App->par->lava, 0, 0);
	volcano_particles->active = false;

	fireshield_timer.Pause();
	fireshield_timer.Reset();

	lavaspawner.Pause();
	lavaspawner.Reset();
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
			App->cam->CameraShake(10, 120);

		}
		charge = 0;
	}

	if (App->inp->GetInput(BUTTON_2) == KEY_REPEAT)
	{
		charge += 2;
	}
	//Fireslash/breath------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && player->grounded)
	{
		player->LockMovement();
		lavaspawner.Start();
		volcano_particles->active = true;
	}

	if (App->inp->GetInput(BUTTON_3) == KEY_REPEAT && player->grounded)
	{
		if (player->is_right)
		{
			volcano_particles->position_x = player->x + player->collider->w;
			volcano_particles->position_y = player->y;
		}
		else
		{
			volcano_particles->position_x = player->x -64;
			volcano_particles->position_y = player->y;
		}
		if(lavaspawner.Read()>timebetweenlava)
		{
		//spawn lava
			LavaSpell* r = new LavaSpell();
			r->blitrect = { 192, 224, 64, 48 };
			r->speed = -6;
			if(player->is_right)
			{
			r->hitbox = { player->collider->x+player->collider->w,player->collider->y+player->collider->h-24,64,48 };
			}
			else
			{
				r->hitbox = { player->collider->x -64,player->collider->y + player->collider->h - 24,64,48 };
			}
			r->parent = this;
			r->life.Start();
			lavaspawner.Reset();
			lavalist.push_back(r);
		}
		/*
			App->ren->Blit(App->tex->Get_Texture("spells"),
				player->collider->x + player->collider->w,
				player->collider->y,
				firebreath_right.GetCurrentFrame(), -2);

			App->ren->Blit(App->tex->Get_Texture("spells"),
				player->collider->x - firebreath_left.GetCurrentFrame()->w,
				player->collider->y,
				firebreath_left.GetCurrentFrame(), -2);
		*/
	}
	for (int i = 0; i < to_delete_lavalist.size(); ++i)
	{
		delete (to_delete_lavalist[i]);
		lavalist.erase(std::find(lavalist.begin(), lavalist.end(), to_delete_lavalist[i]));
	}
	to_delete_lavalist.clear();

	for (int i = 0; i < lavalist.size(); ++i)
	{
		lavalist[i]->Loop();
	}

	if (App->inp->GetInput(BUTTON_3) == KEY_RELEASE)
	{
		player->UnlockMovement();
		volcano_particles->active = false;
	}




	//Fireshield--------------------------------------------------------------------------------------------------------------------
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && !is_fireshield_on_cooldown)
	{
		//player->LockMovement(shield_activation_time);
		is_fireshield_on_cooldown = true;
		fireshield_timer.Start();
		fireshield_timer.Reset();
		to_follow->active = true;
		is_fireshield_up = true;

	}

	if (fireshield_timer.Read() > shield_max_time)
	{
		to_follow->active = false;
		is_fireshield_up = false;
	}

	if (fireshield_timer.Read() > cooldown_shield)
	{
		is_fireshield_on_cooldown = false;
	}


}

void Fire::Render()
{
	if (App->inp->GetInput(BUTTON_2) == KEY_REPEAT)
	{
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

	for (int i = 0; i < lavalist.size(); ++i)
	{
		lavalist[i]->Render();
	}

	if (is_fireshield_up)
	{
		App->ren->Blit(App->tex->Get_Texture("spells"),
			player->collider->x + (player->collider->w - fireshield.GetCurrentFrame()->w) / 2,
			player->collider->y + (player->collider->h - fireshield.GetCurrentFrame()->h) / 2,
			fireshield.GetCurrentFrame(), -2);

		to_follow->position_x = player->x;
		to_follow->position_y = player->y;
	}
}

void Fire::Switched_in()
{
}

void Fire::Switched_out()
{
	
	to_follow->active = false;
	if (is_fireshield_up)
	{
		is_fireshield_up = false;
	}
	charge = 0;
}

void Fire::UnlockMovementEvent()
{

}


void Fire::CleanUp()
{

}

void Fire::DeleteLava(LavaSpell* to_delete)
{
	to_delete_lavalist.push_back(to_delete);
}
