#include "Fire.h"
#include "../GameObjects/Player.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/ObjectManager.h"
#include "../GameObjects/SpellProjectiles/FireBall.h"
#include "Modules/Render.h"
#include "Modules/Camera.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"
#include "../GameObjects/SpellProjectiles/LavaSpell.h"

#include "Utils/Utils.h"

#include "../GameObjects/Enemies/CoalJumper.h"
#include "../GameObjects/Enemies/GroundedElemental.h"
#include "../GameObjects/Enemies/FlyingElemental.h"
#include "../GameObjects/Enemies/ArmorTrap.h"
#include "../GameObjects/Enemies/ShieldMonster.h"
#include "../GameObjects/Enemies/ClingingCreature.h"


Fire::~Fire()
{
	Engine->GetModule<Particles>().RemoveParticleEmitter(volcano_particles);
	Engine->GetModule<Particles>().RemoveParticleEmitter(to_follow);
	//Engine->GetModule<Particles>().to_delete.push_back(volcano_particles);
	//Engine->GetModule<Particles>().to_delete.push_back(to_follow);

	
}

void Fire::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");

	mSFXFireBall = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/fireball_small.wav");
	mSFXFireBallBig = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/fireball_big.wav");
	mSFXFireShield = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/waterfall.wav");
	mSFXFireWaterFall = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/shield2.wav");

	r2shield = { 12,0,12,12 };
	r12shield = { 24,0,12,12 };
	fireshield_part.area_in_texture.push_back(&r2shield);
	fireshield_part.area_in_texture.push_back(&r2shield);
	fireshield_part.area_in_texture.push_back(&r12shield);
	fireshield_part.name = "fireshield";
	fireshield_part.minmax_x_offset = std::make_pair(-5, 69);
	fireshield_part.minmax_y_offset = std::make_pair(0, 70);
	fireshield_part.minmax_speed_y = std::make_pair(-1.5, -2.5);
	fireshield_part.minmax_speed_x = std::make_pair(-0.1, 0.1);
	fireshield_part.minmax_scale_speed = std::make_pair(-0.03, -0.04);
	fireshield_part.minmax_scale = std::make_pair(1, 1.5);
	fireshield_part.minmax_acc_y = std::make_pair(0.04, 0.05);
	fireshield_part.minmax_lifespan = std::make_pair(300, 400);
	fireshield_part.minmax_frequency = std::make_pair(5, 20);
	fireshield_part.texture_name = particles;

	r3lava = { 36,0,12,12 };
	lava.area_in_texture.push_back(&r3lava);
	lava.name = "lava";
	lava.minmax_speed_y = std::make_pair(-3, -4);
	lava.minmax_speed_x = std::make_pair(-2, 2);
	lava.minmax_lifespan = std::make_pair(200, 300);
	lava.minmax_x_offset = std::make_pair(0, 64);
	lava.minmax_y_offset = std::make_pair(60, 64);
	lava.minmax_acc_y = std::make_pair(0.4, 0.5);
	lava.minmax_frequency = std::make_pair(5, 20);
	lava.texture_name = particles;


	firebreath_left.AddFrame({ 224,0,32,64 });
	firebreath_right.AddFrame({ 224,64,32,64 });
	fireshield.AddFrame({ 0,64,96,96 });

	fireball_big = { 0,0,64,64 };
	fireball_small = { 96,0,32,32 };

	to_follow = Engine->GetModule<Particles>().AddParticleEmitter(&fireshield_part,0,0);
	to_follow->active = false;

	volcano_particles = Engine->GetModule<Particles>().AddParticleEmitter(&lava, 0, 0);
	volcano_particles->active = false;

	fireshield_timer.Pause();
	fireshield_timer.Reset();

	lavaspawner.Pause();
	lavaspawner.Reset();
}

void Fire::Loop(float dt)
{
	//Fireball--------------------------------------------------------------------------------------------------------------------------
	if (Engine->GetModule<Input>().GetInput(BUTTON_2) == KEY_RELEASE)
	{

		if (charge > 70 && player->manaCost(manacost_big))
		{
			FireBall* fireball = (FireBall*)Engine->GetModule<ObjectManager>().AddObject(player->collider->x, player->collider->y, 32, 32, GetTypeIndex<FireBall>());
			fireball->Fire(player->is_right, true);
			Engine->GetModule<Camera>().CameraShake(20, 120);
			Engine->GetModule<Audio>().PlaySFX(mSFXFireBall);
			player->AddMana(-manacost_big);
		}
		else if (player->manaCost(manacost_small))
		{
			FireBall* fireball = (FireBall*)Engine->GetModule<ObjectManager>().AddObject(player->collider->x, player->collider->y, 32, 32, GetTypeIndex<FireBall>());
			fireball->Fire(player->is_right, false);
			Engine->GetModule<Camera>().CameraShake(10, 120);
			Engine->GetModule<Audio>().PlaySFX(mSFXFireBallBig);
			player->AddMana(-manacost_small);

		}
		charge = 0;
	}

	if (Engine->GetModule<Input>().GetInput(BUTTON_2) == KEY_REPEAT)
	{
		charge += 2;
	}
	//Fireslash/breath------------------------------------------------------------------------------------------------------------------
	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_DOWN && player->grounded && !Engine->GetModule<ObjectManager>().isPaused())
	{
		player->LockMovement();
		lavaspawner.Start();
		volcano_particles->active = true;
		is_volcano_active = true;
		Engine->GetModule<Audio>().PlaySFX(mSFXFireWaterFall, -1,audiochannel_volcano);
		
	}

	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_REPEAT && player->grounded && is_volcano_active && player->manaCost(manacost_volc))
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
			LavaSpell* r = new LavaSpell(spells,Engine);
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

			player->AddMana(-0.2);
		}
		/*
			Engine->GetModule<::Render>().Blit(Engine->GetModule<Textures>().Get_Texture("spells"),
				player->collider->x + player->collider->w,
				player->collider->y,
				firebreath_right.GetCurrentFrame(), -2);

			Engine->GetModule<::Render>().Blit(Engine->GetModule<Textures>().Get_Texture("spells"),
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

	if (Engine->GetModule<Input>().GetInput(BUTTON_3) == KEY_RELEASE)
	{
		player->UnlockMovement();
		volcano_particles->active = false;
		is_volcano_active = false;
		Engine->GetModule<Audio>().StopChannel(audiochannel_volcano);
	}

	//Fireshield--------------------------------------------------------------------------------------------------------------------
	if (Engine->GetModule<Input>().GetInput(BUTTON_4) == KEY_DOWN && !is_fireshield_on_cooldown && player->manaCost(manacost_shield) && !Engine->GetModule<ObjectManager>().isPaused())
	{
		//player->LockMovement(shield_activation_time);
		is_fireshield_on_cooldown = true;
		fireshield_timer.Start();
		fireshield_timer.Reset();
		to_follow->active = true;
		is_fireshield_up = true;

		player->AddMana(-manacost_shield);


		audiochannel_shield = Engine->GetModule<Audio>().PlaySFX(mSFXFireShield,8);
	}

	if (fireshield_timer.Read() > shield_max_time)
	{
		to_follow->active = false;
		is_fireshield_up = false;
		//Engine->GetModule<Audio>().StopChannel(audiochannel_shield);
	}

	if (fireshield_timer.Read() > cooldown_shield)
	{
		is_fireshield_on_cooldown = false;
	}

	if (is_fireshield_up)
	{
		player->AddMana(-manacost_shield_over_time);

		RXRect fireshield = {player->collider->x-16,player->collider->y-16,96,96};
		std::vector<collision*> collisions;
		Engine->GetModule<ObjectManager>().GetCollisions(&fireshield, collisions);

		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it)->object != player)
			{
				int direction = 0;
				if (player->collider->x < (*it)->object->collider->x)
				{
					direction = 1;
				}
				else
				{
					direction = -1;
				}

				if ((*it)->object->IsSameTypeAs<Enemy>())
				{
					((Enemy*)(*it)->object)->RecieveDamage(0.3, direction);
				}
			}
		}
	}
	
}

void Fire::Render()
{
	if (Engine->GetModule<Input>().GetInput(BUTTON_2) == KEY_REPEAT)
	{
		if (!Engine->GetModule<ObjectManager>().isPaused())
		{
			if (charge > 100)
			{
				if (player->is_right)
				{
					Engine->GetModule<::Render>().RenderTexture(spells,
						player->collider->x + player->collider->w - 5,
						player->collider->y + player->collider->h / 2 - fireball_big.h / 2,
						fireball_big, -2);
				}
				else
				{
					Engine->GetModule<::Render>().RenderTexture(spells,
						player->collider->x - fireball_big.w + 5,
						player->collider->y + player->collider->h / 2 - fireball_big.h / 2,
						fireball_big, -2);
				}
			}
			else
			{
				if (player->is_right)
				{
					Engine->GetModule<::Render>().RenderTexture(spells,
						player->collider->x + player->collider->w - 15,
						player->collider->y + player->collider->h / 2 - fireball_small.h / 2,
						fireball_small, -2);
				}
				else
				{
					Engine->GetModule<::Render>().RenderTexture(spells,
						player->collider->x - fireball_small.w + 15,
						player->collider->y + player->collider->h / 2 - fireball_small.h / 2,
						fireball_small, -2);
				}
			}
		}
	}

	for (int i = 0; i < lavalist.size(); ++i)
	{
		lavalist[i]->Render();
	}

	if (is_fireshield_up)
	{
		Engine->GetModule<::Render>().RenderTexture(spells,
			player->collider->x + (player->collider->w - fireshield.GetCurrentFrame().w) / 2,
			player->collider->y + (player->collider->h - fireshield.GetCurrentFrame().h) / 2,
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

	if (is_volcano_active)
	{
		player->UnlockMovement();
		volcano_particles->active = false;
		is_volcano_active = false;
		Engine->GetModule<Audio>().StopChannel(audiochannel_volcano);

		for (int i = 0; i < lavalist.size(); ++i)
		{
			delete (lavalist[i]);
			//lavalist.erase(std::find(lavalist.begin(), lavalist.end(), to_delete_lavalist[i]));
		}
		lavalist.clear();
		to_delete_lavalist.clear();
	}
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
