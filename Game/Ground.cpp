#include "Ground.h"
#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Camera.h"
#include "Particles.h"
#include "Audio.h"

#include "Rock.h"
#include "Shockwave.h"

#include "CoalJumper.h"
#include "GroundedElemental.h"
#include "FlyingElemental.h"
#include "ArmorTrap.h"
#include "ShieldMonster.h"
#include "ClingingCreature.h"

void Ground::Init()
{
	groundpoundhitbox.w = 64;
	groundpoundhitbox.h = 32;
}

void Ground::Loop(float dt)
{

	//rock throw--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN && ! is_rock_on_cooldown)
	if (App->inp->GetInput(BUTTON_2) == KEY_DOWN && !is_rock_on_cooldown && player->manaCost(manacost_rock) && !App->phy->is_paused)
	{
			((Rock*)App->phy->AddObject(player->collider->x+player->collider->w/2, player->collider->y+player->collider->h / 2, 32, 32, ROCK))->Fire(player->is_right,45,15,1);
			is_rock_on_cooldown = true;
			rock_timer.Reset();
			rock_timer.Start();
			player->AddMana(-manacost_rock);
			App->aud->PlaySFX(SFX_ROCK_THROW);
	}
	
	if(is_rock_on_cooldown && rock_timer.Read()> cooldown_rock)
	{
		is_rock_on_cooldown = false;
	}

	
	//groundpound------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && !groundpounding)
	if (App->inp->GetInput(BUTTON_3) == KEY_DOWN && !groundpounding&& player->manaCost(manacost_groundpound) && !App->phy->is_paused)
	{
		current_yspeed = initial_yspeed;
		groundpounding = true;
		player->LockMovement();
		App->aud->PlaySFX(SFX_GROUNDPOUND_START);

		player->AddMana(-manacost_groundpound);

	}

	if (groundpounding)
	{
		groundpoundhitbox.x = player->collider->x;
		groundpoundhitbox.y = player->collider->y + player->collider->h;


		std::vector<collision*> collisions;
		App->phy->GetCollisions(&groundpoundhitbox, collisions);
		
		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if (!hashitsomething)
			{
				if ((*it)->type == COAL_JUMPER)
				{
					((CoalJumper*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
				if ((*it)->type == GROUNDED_ELEMENTAL)
				{
					((GroundedElemental*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
				if ((*it)->type == FLYING_ELEMENTAL)
				{
					((FlyingElemental*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
				if ((*it)->type == ARMOR_TRAP)
				{
					((ArmorTrap*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
				if ((*it)->type == SHIELD_MONSTER)
				{
					((ShieldMonster*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
				if ((*it)->type == CLING_CREATURE)
				{
					((ClingCreature*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
			}
			if ((*it)->type == HAZARDS_ROCK_BLOCK)
			{
				App->phy->DeleteObject((*it)->object);
				App->par->AddParticleEmitter(&App->par->rockblockexplosion, ((*it)->object)->collider->x + ((*it)->object)->collider->w / 2, ((*it)->object)->collider->y + ((*it)->object)->collider->h / 2, 300);

			}
		}

		App->phy->ClearCollisionArray(collisions);

		// move player accordingly
		if (!is_on_gp_lag)
		{
			player->nextpos->y += current_yspeed;
			current_yspeed += gravity;
		}
		//check for floor and stop once on it
		std::vector<SDL_Rect*> colliders;
		App->phy->GetNearbyWalls(player->nextpos->x + player->nextpos->w / 2, player->nextpos->y + player->nextpos->h / 2, 300, colliders);

		for (int i = 0; i < colliders.size(); ++i)
		{
			SDL_Rect hitbox = {player->nextpos->x,player->nextpos->y+player->nextpos->h,player->nextpos->w,32};
			SDL_Rect result;
			if (current_yspeed > 0 &&!is_on_gp_lag && SDL_IntersectRect(colliders[i], &hitbox, &result) == SDL_TRUE)// he goin crash!
			{
				player->nextpos->y -= result.h;
				is_on_gp_lag = true;
				//start timer
				groundpound_end_timer.Reset();
				groundpound_end_timer.Start();
				current_yspeed = 0;

				App->cam->CameraShake(35, 0.7);//ADD PARTICLES
				App->par->AddParticleEmitter(&App->par->groundcontact, hitbox.x, hitbox.y + hitbox.h, 200);
				App->par->AddParticleEmitter(&App->par->groundcontact, hitbox.x+hitbox.w/2, hitbox.y + hitbox.h, 200);
				App->par->AddParticleEmitter(&App->par->groundcontact, hitbox.x+hitbox.w, hitbox.y + hitbox.h, 200);
			}
		}
	}

	if(is_on_gp_lag && groundpound_end_timer.Read() > groundpound_endlag)
	{
		groundpounding = false;
		is_on_gp_lag = false;
		player->UnlockMovement();
	}

	
	//earthquake--------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(B) == BUTTON_DOWN && player->grounded && !is_eq_on_cooldown)
	if (App->inp->GetInput(BUTTON_4) == KEY_DOWN && player->grounded && !is_eq_on_cooldown && player->manaCost(manacost_earthquake) && !App->phy->is_paused)
	{
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE))->Fire(true, 8);
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE))->Fire(false, 8);
		
		is_eq_on_cooldown = true;
		earthquake_timer.Reset();
		earthquake_timer.Start();

		App->aud->PlaySFX(SFX_SHOCKWAVE);
		player->AddMana(-manacost_earthquake);

		App->cam->CameraShake(15, 2);//ADD PARTICLES
		App->par->AddParticleEmitter(&App->par->groundcontact, player->collider->x, player->collider->y + player->collider->h, 200);
		App->par->AddParticleEmitter(&App->par->groundcontact, player->collider->x + player->collider->w, player->collider->y + player->collider->h, 200);
	}

	if (is_eq_on_cooldown && earthquake_timer.Read() > cooldown_earthquake)
	{
		is_eq_on_cooldown = false;
	}
}

void Ground::Render()
{
	if (groundpounding)
	{
		//add graphic
		App->ren->Blit(App->tex->Get_Texture("spells"), player->collider->x, player->collider->y + player->collider->h, &groundpound, -2);
	}
}

void Ground::Switched_in()
{
}

void Ground::Switched_out()
{
	groundpounding = false;
	is_on_gp_lag = false;
	player->UnlockMovement();
}

void Ground::UnlockMovementEvent()
{

}


void Ground::CleanUp()
{

}