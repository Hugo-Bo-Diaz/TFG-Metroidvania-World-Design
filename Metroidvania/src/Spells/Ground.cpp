#include "Ground.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/ObjectManager.h"
#include "Modules/Render.h"
#include "Modules/Camera.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"
#include "Utils/Utils.h"

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

#include "../GameObjects/Hazards/HazardsRockBlock.h"

void Ground::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

	mSFXGroundPound = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/groundpound.wav");
	mSFXRock = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/rock_throw.wav");
	mSFXShockwave = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/shockwave.wav.wav");

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
	rockblockexplosion.texture_name = particles;

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
	groundcontact.texture_name = particles;

}

void Ground::Loop(float dt)
{

	//rock throw--------------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(X) == BUTTON_DOWN && ! is_rock_on_cooldown)
	if (Engine->GetModule<Input>().GetInput(BUTTON_2) == KEY_DOWN && !is_rock_on_cooldown && player->manaCost(manacost_rock) && !Engine->GetModule<ObjectManager>().isPaused())
	{
			((Rock*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x+player->collider.w/2, player->collider.y+player->collider.h / 2, 32, 32, GetTypeIndex<Rock>()))->Fire(player->is_right,45,15,1);
			is_rock_on_cooldown = true;
			rock_timer.Reset();
			rock_timer.Start();
			player->AddMana(-manacost_rock);
			Engine->GetModule<Audio>().PlaySFX(mSFXRock);
	}
	
	if(is_rock_on_cooldown && rock_timer.Read()> cooldown_rock)
	{
		is_rock_on_cooldown = false;
	}

	
	//groundpound------------------------------------------------------------------------------------------------------------------
	//if (App->inp->GetButton(Y) == BUTTON_DOWN && !groundpounding)
	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_DOWN && !groundpounding&& player->manaCost(manacost_groundpound) && !Engine->GetModule<ObjectManager>().isPaused())
	{
		current_yspeed = initial_yspeed;
		groundpounding = true;
		player->LockMovement();
		Engine->GetModule<Audio>().PlaySFX(mSFXGroundPound);

		player->AddMana(-manacost_groundpound);

	}

	if (groundpounding)
	{
		groundpoundhitbox.x = player->collider.x;
		groundpoundhitbox.y = player->collider.y + player->collider.h;


		std::vector<collision*> collisions;
		Engine->GetModule<ObjectManager>().GetCollisions(&groundpoundhitbox, collisions);
		
		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if (!hashitsomething)
			{
				if ((*it)->object->IsSameTypeAs<Enemy>())
				{
					((Enemy*)(*it)->object)->RecieveDamage(5, player->is_right);
				}
			}
			if ((*it)->object->IsSameTypeAs<HazardRockBlock>())
			{
				Engine->GetModule<ObjectManager>().DeleteObject((*it)->object);
				Engine->GetModule<Particles>().AddParticleEmitter(&rockblockexplosion, ((*it)->object)->collider.x + ((*it)->object)->collider.w / 2, ((*it)->object)->collider.y + ((*it)->object)->collider.h / 2, 300);

			}
		}

		Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

		// move player accordingly
		if (!is_on_gp_lag)
		{
			player->nextpos->y += current_yspeed;
			current_yspeed += gravity;
		}
		//check for floor and stop once on it
		std::vector<RXRect*> colliders;
		Engine->GetModule<ObjectManager>().GetNearbyWalls(player->nextpos->x + player->nextpos->w / 2, player->nextpos->y + player->nextpos->h / 2, 300, colliders);

		for (int i = 0; i < colliders.size(); ++i)
		{
			RXRect hitbox = {player->nextpos->x,player->nextpos->y+player->nextpos->h,player->nextpos->w,32};
			RXRect result;
			if (current_yspeed > 0 &&!is_on_gp_lag && RXRectCollision(colliders[i], &hitbox, &result) == true)// he goin crash!
			{
				player->nextpos->y -= result.h;
				is_on_gp_lag = true;
				//start timer
				groundpound_end_timer.Reset();
				groundpound_end_timer.Start();
				current_yspeed = 0;

				Engine->GetModule<Camera>().CameraShake(35, 0.7);//ADD PARTICLES
				Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, hitbox.x, hitbox.y + hitbox.h, 200);
				Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, hitbox.x+hitbox.w/2, hitbox.y + hitbox.h, 200);
				Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, hitbox.x+hitbox.w, hitbox.y + hitbox.h, 200);
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
	if (Engine->GetModule<Input>().GetInput(BUTTON_4) == KEY_DOWN && player->grounded && !is_eq_on_cooldown && player->manaCost(manacost_earthquake) && !Engine->GetModule<ObjectManager>().isPaused())
	{
		((Shockwave*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x + player->collider.w / 2, player->collider.y + player->collider.h-32, 32, 32, GetTypeIndex<Shockwave>()))->Fire(true, 8);
		((Shockwave*)Engine->GetModule<ObjectManager>().AddObject(player->collider.x + player->collider.w / 2, player->collider.y + player->collider.h-32, 32, 32, GetTypeIndex<Shockwave>()))->Fire(false, 8);
		
		is_eq_on_cooldown = true;
		earthquake_timer.Reset();
		earthquake_timer.Start();

		Engine->GetModule<Audio>().PlaySFX(mSFXShockwave);
		player->AddMana(-manacost_earthquake);

		Engine->GetModule<Camera>().CameraShake(15, 2);//ADD PARTICLES
		Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, player->collider.x, player->collider.y + player->collider.h, 200);
		Engine->GetModule<Particles>().AddParticleEmitter(&groundcontact, player->collider.x + player->collider.w, player->collider.y + player->collider.h, 200);
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
		Engine->GetModule<::Render>().RenderTexture(spells, player->collider.x, player->collider.y + player->collider.h, groundpound, -2);
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