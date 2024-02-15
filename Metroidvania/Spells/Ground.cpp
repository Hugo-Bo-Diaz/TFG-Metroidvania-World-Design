#include "Ground.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Input.h"
#include "Physics.h"
#include "Render.h"
#include "Camera.h"
#include "Particles.h"
#include "Audio.h"

#include "../GameObjects/SpellProjectiles/Rock.h"
#include "../GameObjects/SpellProjectiles/Shockwave.h"

#include "../GameObjects/Enemies/CoalJumper.h"
#include "../GameObjects/Enemies/GroundedElemental.h"
#include "../GameObjects/Enemies/FlyingElemental.h"
#include "../GameObjects/Enemies/ArmorTrap.h"
#include "../GameObjects/Enemies/ShieldMonster.h"
#include "../GameObjects/Enemies/ClingingCreature.h"
#include "../GameObjects/Enemies/FlyingAxe.h"
#include "../GameObjects/Enemies/FlyingShield.h"
#include "../GameObjects/EntityIDs.h"

void Ground::Init()
{
	groundpoundhitbox.w = 64;
	groundpoundhitbox.h = 32;

	r8ground = { 0,24,12,12 };
	r9ground = { 12,24,12,12 };
	rockblockexplosion.area_in_texture.push_back(&r8ground);
	rockblockexplosion.area_in_texture.push_back(&r9ground);
	rockblockexplosion.name = "ground";
	rockblockexplosion.minmax_speed_y = std::make_pair(-6, -4);
	rockblockexplosion.minmax_speed_x = std::make_pair(-3, 3);
	rockblockexplosion.minmax_lifespan = std::make_pair(75, 150);
	rockblockexplosion.minmax_angle_speed = std::make_pair(5, 15);
	rockblockexplosion.minmax_angle = std::make_pair(0, 360);
	rockblockexplosion.minmax_x_offset = std::make_pair(-25, 25);
	rockblockexplosion.minmax_y_offset = std::make_pair(-25, 25);
	rockblockexplosion.minmax_acc_y = std::make_pair(1, 2);
	rockblockexplosion.minmax_frequency = std::make_pair(10, 25);
	rockblockexplosion.texture_name = "particles";


	groundcontact.area_in_texture.push_back(&r8ground);
	groundcontact.area_in_texture.push_back(&r9ground);
	groundcontact.name = "ground";
	groundcontact.minmax_speed_y = std::make_pair(-6, -4);
	groundcontact.minmax_speed_x = std::make_pair(-3, 3);
	groundcontact.minmax_lifespan = std::make_pair(75, 150);
	groundcontact.minmax_angle_speed = std::make_pair(5, 15);
	groundcontact.minmax_angle = std::make_pair(0, 360);
	groundcontact.minmax_x_offset = std::make_pair(-15, 15);
	groundcontact.minmax_y_offset = std::make_pair(-15, 15);
	groundcontact.minmax_acc_y = std::make_pair(1, 2);
	groundcontact.minmax_frequency = std::make_pair(10, 25);
	groundcontact.texture_name = "particles";

}

void Ground::Loop(float dt)
{

	//rock throw--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN && ! is_rock_on_cooldown)
	if (App->inp->GetInput(BUTTON_2) == KEY_DOWN && !is_rock_on_cooldown && player->manaCost(manacost_rock) && !App->phy->is_paused)
	{
			((Rock*)App->phy->AddObject(player->collider->x+player->collider->w/2, player->collider->y+player->collider->h / 2, 32, 32, ROCK_ID))->Fire(player->is_right,45,15,1);
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
				objectId t = (*it)->type;
				if (t == COAL_JUMPER_ID || t == GROUNDED_ELEMENTAL_ID || t == FLYING_ELEMENTAL_ID ||
					t == ARMOR_TRAP_ID || t == SHIELD_MONSTER_ID || t == CLING_CREATURE_ID ||
					t == FLYING_AXE_ID || t == FLYING_SHIELD_ID)
				{
					((Enemy*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
			}
			if ((*it)->type == HAZARDS_ROCK_BLOCK_ID)
			{
				App->phy->DeleteObject((*it)->object);
				App->par->AddParticleEmitter(&rockblockexplosion, ((*it)->object)->collider->x + ((*it)->object)->collider->w / 2, ((*it)->object)->collider->y + ((*it)->object)->collider->h / 2, 300);

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
				App->par->AddParticleEmitter(&groundcontact, hitbox.x, hitbox.y + hitbox.h, 200);
				App->par->AddParticleEmitter(&groundcontact, hitbox.x+hitbox.w/2, hitbox.y + hitbox.h, 200);
				App->par->AddParticleEmitter(&groundcontact, hitbox.x+hitbox.w, hitbox.y + hitbox.h, 200);
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
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE_ID))->Fire(true, 8);
		((Shockwave*)App->phy->AddObject(player->collider->x + player->collider->w / 2, player->collider->y + player->collider->h-32, 32, 32, SHOCKWAVE_ID))->Fire(false, 8);
		
		is_eq_on_cooldown = true;
		earthquake_timer.Reset();
		earthquake_timer.Start();

		App->aud->PlaySFX(SFX_SHOCKWAVE);
		player->AddMana(-manacost_earthquake);

		App->cam->CameraShake(15, 2);//ADD PARTICLES
		App->par->AddParticleEmitter(&groundcontact, player->collider->x, player->collider->y + player->collider->h, 200);
		App->par->AddParticleEmitter(&groundcontact, player->collider->x + player->collider->w, player->collider->y + player->collider->h, 200);
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