#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Logger.h"
#include "Camera.h"
#include "SceneController.h"
#include "ProgressTracker.h"

#include "Window.h"
#include "Audio.h"

#include "Gui.h"
#include "../UIElements/UItextbox.h"

#include "../Spells/Fire.h"
#include "../Spells/Wind.h"
#include "../Spells/Water.h"
#include "../Spells/Grass.h"
#include "../Spells/Ground.h"
#include "EntityIDs.h"

Player::Player()
{
	playerTex = App->tex->Load_Texture("Assets/Sprites/Player.png");

	speed_y = 0;
	acceleration_y = 0.5;

	idle_right.AddFrame({0,0,64,64});
	idle_right.AddFrame({ 0,0,64,64 });
	idle_right.AddFrame({ 64,0,64,64 });
	idle_right.AddFrame({ 64,0,64,64 });
	idle_right.AddFrame({ 128,0,64,64 });
	idle_right.AddFrame({ 128,0,64,64 });
	idle_right.AddFrame({ 192,0,64,64 });
	idle_right.AddFrame({ 192,0,64,64 });

	idle_left.AddFrame({ 0, 64,64,64 });
	idle_left.AddFrame({ 0, 64,64,64 });
	idle_left.AddFrame({ 64,64,64,64 });
	idle_left.AddFrame({ 64,64,64,64 });
	idle_left.AddFrame({ 128,64,64,64 });
	idle_left.AddFrame({ 128,64,64,64 });
	idle_left.AddFrame({ 192,64,64,64 });
	idle_left.AddFrame({ 192,64,64,64 });

	walking_right.AddFrame({ 0, 128,64,64 });
	walking_right.AddFrame({ 64,128,64,64 });
	walking_right.AddFrame({ 128,128,64,64 });
	walking_right.AddFrame({ 192,128,64,64 });

	walking_left.AddFrame({ 0, 192,64,64 });
	walking_left.AddFrame({ 64,192,64,64 });
	walking_left.AddFrame({ 128,192,64,64 });
	walking_left.AddFrame({ 192,192,64,64 });

	air_right.AddFrame({ 0, 256,64,64 });
	air_right.AddFrame({ 0, 256,64,64 });
	air_right.AddFrame({ 64,256,64,64 });
	air_right.AddFrame({ 64,256,64,64 });
	air_right.AddFrame({ 128,256,64,64 });
	air_right.AddFrame({ 128,256,64,64 });
	air_right.AddFrame({ 192,256,64,64 });
	air_right.AddFrame({ 192,256,64,64 });

	air_left.AddFrame({ 0, 320,64,64 });
	air_left.AddFrame({ 0, 320,64,64 });
	air_left.AddFrame({ 64,320,64,64 });
	air_left.AddFrame({ 64,320,64,64 });
	air_left.AddFrame({ 128,320,64,64 });
	air_left.AddFrame({ 128,320,64,64 });
	air_left.AddFrame({ 192,320,64,64 });
	air_left.AddFrame({ 192,320,64,64 });

	//pos_x = App->con->AddVar_track("position x", &x);
	//pos_y = App->con->AddVar_track("position y", &y);
	//xspeed = App->con->AddVar_track("speed", &spd);

	for (int i = 0; i < 29; ++i)
	{
		trail[i].x = 0;
		trail[i].y = 0;
	}
	App->cam->FollowObject(this);

	Fire* fire_spell = new Fire();
	fire_spell->Init();
	fire_spell->player = this;
	spells.push_back(fire_spell);
	Wind* wind_spell = new Wind();
	wind_spell->Init();
	wind_spell->player = this;
	spells.push_back(wind_spell);
	Water* water_spell = new Water();
	water_spell->Init();
	water_spell->player = this;
	spells.push_back(water_spell);
	Grass* grass_spell = new Grass();
	grass_spell->Init();
	grass_spell->player = this;
	spells.push_back(grass_spell);
	Ground* ground_spell = new Ground();
	ground_spell->Init();
	ground_spell->player = this;
	spells.push_back(ground_spell);

	unlocked_spells =0;
	
	unlocked.push_back(true);
	unlocked.push_back(false);
	unlocked.push_back(false);
	unlocked.push_back(false);
	unlocked.push_back(false);
	
	current_spell = FIRE;

	/*unlocked.push_back(true);
	unlocked.push_back(true);
	unlocked.push_back(true);
	unlocked.push_back(true);
	unlocked.push_back(true);
	*/

	if (App->trk->BaseSaveSection->GetChild("Stats") == nullptr)
	{
		App->trk->BaseSaveSection->AddNewChild("Stats");
	}
}

void Player::Init()
{
	nextpos = new SDL_Rect();
	nextpos->x = collider->x;
	nextpos->y = collider->y;
	nextpos->w = collider->w;
	nextpos->h = collider->h;
}

bool Player::Loop(float dt)
{

	/*if(unlocked[FIRE]==false)
	{
		unlock_spell(FIRE);
	}*/

	bool ret = true;

	spd = speed_x;

	x = collider->x;
	y = collider->y;

	/*if (App->inp->GetInput(BUTTON_4)==BUTTON_DOWN)
	{
		UItextbox* box = (UItextbox*)App->gui->AddTextBox("authorlol", "this is a test", RED, 15, 4, 272, 420, 2, 0.2);

		box->AddPanelToTextBox("i used to roll the dice");
		box->AddPanelToTextBox("feel my in my enemy");
	}*/

	/*
	STEPS:
	STEP 1: collider to its next frame calculated position (calculated previous frame)
	STEP 2: calculate next frame position
	STEP 3: correct next frame position
	STEP 4: profit
	*/

	//if (App->inp->GetInput(BUTTON_4) == BUTTON_DOWN)
	//{
	//	//AddHealth(-4);
	//	//App->win->ToggleFullScreen();
	//	App->trk->LoadGame("save_file.xml");
	//}
	//if (App->inp->GetInput(BUTTON_3) == BUTTON_DOWN)
	//{
	//	//AddHealth(-4);
	//	//App->win->ToggleFullScreen();
	//	App->trk->SaveGame("save_file.xml");
	//}

	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_x += acceleration_x;
	speed_y += acceleration_y;
	if (speed_y > speed_y_cap)
		speed_y = speed_y_cap;
	//if (abs(speed_y) > 5)
	//{grounded = false;}

	if (movement_is_locked)
	{
		speed_x = 0;
		speed_y = 0;
	}

	nextpos->x += speed_x*mobility_multiplier;
	//if (!grounded)
	nextpos->y += speed_y*mobility_multiplier;
	if (speed_y > acceleration_y)
	{
		grounded = false;
	}

	//STEP 3 oh boi
	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{

			if (result.h > result.w)
			{
				speed_x = 0;
				if (colliders[i]->x > nextpos->x)//right collision
				{
					nextpos->x -= result.w;
				}
				else//left collision
				{
					nextpos->x += result.w;
				}
			}
			if (result.h < result.w)
			{
				if (colliders[i]->y > nextpos->y)//down collision
				{
					if (speed_y > 0)
					{
						nextpos->y -= result.h;
						grounded = true;
						speed_y = 0;
					}
				}
				else
				{
					speed_y = 0;
					nextpos->y += result.h;
				}
			}
		}
	}

	float joyx;
	float joyy;
	App->inp->GetJoystick(true, joyx, joyy);

	if (joyx < -0.3)
	{
		left = true;
	}
	else
	{
		left = false;
	}

	if (joyx > 0.3)
	{
		right = true;
	}
	else
	{
		right = false;
	}


	if (App->inp->GetInput(BUTTON_1) == KEY_DOWN && (grounded))
	{
		jump = true;
	}
	else
	{
		jump = false;
	}

	if (joyx > -0.3 && joyx < 0.3)
	{
		stop = true;
	}
	else
	{
		stop = false;
	}

	if (left && (!is_invincible || GetInvincibilityPercent() > 50))//if recieved msg for left then do this
	{
		//		speed_x = -max_speed;
		if (speed_x > 0)
		{
			speed_x -= accel_x;
		}
		
		if (speed_x > -max_speed)
		{
			speed_x -= accel_x;
		}

		is_right = false;

		current_state = WALKING_LEFT;

		if (!grounded)
		{
			current_state = AIRBORNE_LEFT;
			if (speed_x > -max_speed && abs(speed_y)<4)
			{
				speed_x -= accel_x;
			}
		}

	}

	if (right &&( !is_invincible || GetInvincibilityPercent()>50))//if recieved msg for right then do this
	{
		//speed_x = max_speed;
		
		if (speed_x < 0)
		{
			speed_x += accel_x;
		}

		if (speed_x < max_speed)
		{
			speed_x += accel_x;
		}

		is_right = true;

		current_state = WALKING_RIGHT;
		if (!grounded)
		{
			current_state = AIRBORNE_RIGHT;

			if (speed_x < max_speed && abs(speed_y)<4)
			{
				speed_x += accel_x;
			}
		}
	}


	if (stop && (!is_invincible || GetInvincibilityPercent()>50))
	{
		if (speed_x > 0)
		{
			speed_x -= accel_x;
		}
		else if (speed_x < 0)
		{
			speed_x += accel_x;
		}

		if (grounded)
			current_state = IDLE;
	}

	if (jump)
	{
		speed_y = -13;
		grounded = false;
		acceleration_y = 0.5;

		App->aud->PlaySFX(SFX_JUMP);
	}

	if (speed_y>1||(App->inp->GetInput(BUTTON_1) == BUTTON_RELEASE && grounded == false))
	{
		if (speed_y < -2)
		{
			speed_y = -2;
		}

		acceleration_y = 1.0;
	}

	//if ((App->inp->GetInput(BUTTON_4) == BUTTON_RELEASE))
	//{
	//	AddHealth(1);
	//}

	//if ((App->inp->GetButton(Y) == BUTTON_RELEASE))
	//{
	//	UnlockMovement();
	//}


	//the habilities are handled here

	if (current_spell != NONE_UNLOCKED)
	{
		spells[current_spell]->Loop(dt);
	}
	if (unlocked_spells > 1)
	{
		if (App->inp->GetInput(L_SHOULDER) == KEY_DOWN)
		{
			cycle_spell(-1);
		}
		if (App->inp->GetInput(R_SHOULDER) == KEY_DOWN)//START
		{
			cycle_spell(1);
		}
	}
	
	if (App->inp->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		unlock_spell(FIRE);
	}

	//RENDER

	
	//trail
	for (int i = 29; i > 0; --i)
	{
		trail[i].x = trail[i-1].x;
		trail[i].y = trail[i-1].y;
	}

	trail[0].x = collider->x+collider->w/2;
	trail[0].y = collider->y+collider->h/2;

	//App->ren->DrawTrail(trail, 30);


	//handling lock movement
	if (movement_is_locked && time_to_lock_movement != -1 && movement_lock.Read() > time_to_lock_movement)
	{
		UnlockMovement();
	}

	if (is_invincible &&GetInvincibilityPercent() >= 100)
	{
		is_invincible = false;

	}

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == COAL_JUMPER_ID ||
				(*it)->type == GROUNDED_ELEMENTAL_ID ||
				(*it)->type == FLYING_ELEMENTAL_ID ||
				(*it)->type == ARMOR_TRAP_ID ||
				(*it)->type == SHIELD_MONSTER_ID ||
				(*it)->type == CLING_CREATURE_ID)
			{
				if ((*it)->object->collider->x < collider->x)
				{
					AddHealth(-1, 1);
				}
				else
				{
					AddHealth(-1, -1);
				}
			}
		}
	}

	//handle mana
	if (mana < max_mana && time_since_mana_use.Read() > time_to_start_regen)
	{
		mana += base_mana_regen + ( mana / max_mana )*scaling_mana_regen;
		App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentMana",mana);
	}

	if (mana > max_mana)
	{
		mana = max_mana;
		App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentMana", mana);
	}


	return ret;
}

void Player::LockMovement(float time_in_ms)
{
	if (time_in_ms != -1)
	{
		movement_lock.Start();
		time_to_lock_movement = time_in_ms;
	}

	time_to_lock_movement = time_in_ms;
	movement_is_locked = true;
}

void Player::UnlockMovement()
{
	movement_is_locked = false;
	movement_lock.Pause();
	movement_lock.Reset();

	spells[current_spell]->UnlockMovementEvent();
}

void Player::StartInvincibility()
{
	invincibility.Reset();
	is_invincible = true;
}

float Player::GetInvincibilityPercent()
{
	return (invincibility.Read() / time_invincible)*100;
}

void Player::unlock_spell(spell_type to_unlock)
{
	unlocked[(int)to_unlock] = true;
	current_spell = to_unlock;

	unlocked_spells = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (unlocked[i] == true)
			unlocked_spells++;
	}
}

void Player::cycle_spell(int direction)
{
	if (current_spell != NONE_UNLOCKED)
	{
		spells[current_spell]->Switched_out();

		int iterator = current_spell;
		bool exit = false;
		while (!exit)
		{
			iterator += direction;

			if (iterator >= 5)
			{
				iterator = 0;
			}
			if (iterator <= -1)
			{
				iterator = 4;
			}

			if (unlocked[iterator])
			{
				current_spell = (spell_type)iterator;
				spells[current_spell]->Switched_in();
				exit = true;
			}
		}
	}
}

int Player::get_next_spell(int direction)
{

	int iterator = current_spell;
	bool exit = false;
	while (!exit)
	{
		iterator += direction;

		if (iterator >= 5)
		{
			iterator = 0;
		}
		if (iterator <= -1)
		{
			iterator = 4;
		}

		if (unlocked[iterator])
		{
			exit = true;
		}
	}
	return iterator;
}

bool Player::manaCost(float _mana)
{
	if (mana > _mana)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::AddHealth(int amount, int knockbackdirection)
{
	if (!App->debug)
	{
		if (!is_invincible || amount > 0)
		{
			health += amount;
			if (health > max_health)
			{
				health = max_health;
			}
		}
	}
	App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentHP", health);


	if (amount < 0 && !is_invincible)
	{
		App->aud->PlaySFX(SFX_PLAYER_HIT);
	}

	if (!is_invincible && amount < 0 && knockbackdirection !=0)
	{

		speed_y = -12;
		speed_x = 6* knockbackdirection;
		StartInvincibility();
		invin_draw_timer.Reset();
		App->cam->CameraShake(20,100);
	}
}

void Player::AddMana(float amount)
{
	mana += amount;
	App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentMana", mana);
	time_since_mana_use.Reset();
	if(mana<0)
	{
		mana = 0;
	}
}

void Player::AddMaxHealth()
{
	int currentHPfrag = App->trk->BaseSaveSection->GetChild("Stats")->GetValue("CurrentFragmentsHP");
	App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentFragmentsHP", currentHPfrag + 1);
}

void Player::AddMaxMana()
{
	int currentHPfrag = App->trk->BaseSaveSection->GetChild("Stats")->GetValue("CurrentFragmentsMana");
	App->trk->BaseSaveSection->GetChild("Stats")->SetValue("CurrentFragmentsMana", currentHPfrag);
}

bool Player::Render()
{
	if (ShouldDraw())
	{
		switch (current_state)
		{
		case IDLE:
		{
			if (is_right)
			{
				App->ren->Blit(playerTex, collider->x-separation, collider->y, idle_right.GetCurrentFrame(), 0);
				idle_right.NextFrame();
			}
			else
			{
				App->ren->Blit(playerTex, collider->x-separation, collider->y, idle_left.GetCurrentFrame(), 0);
				idle_left.NextFrame();
			}
		}
		break;
		case WALKING_LEFT:
		{
			App->ren->Blit(playerTex, collider->x-separation, collider->y, walking_left.GetCurrentFrame(), 0);
			walking_left.NextFrame();
		}
		break;
		case WALKING_RIGHT:
		{
			App->ren->Blit(playerTex, collider->x-separation, collider->y, walking_right.GetCurrentFrame(), 0);
			walking_right.NextFrame();
		}
		break;
		case AIRBORNE_LEFT:
		{
			App->ren->Blit(playerTex, collider->x-separation, collider->y, air_left.GetCurrentFrame(), 0);
			air_left.NextFrame();
		}
		break;
		case AIRBORNE_RIGHT:
		{
			App->ren->Blit(playerTex, collider->x-separation, collider->y, air_right.GetCurrentFrame(), 0);
			air_right.NextFrame();
		}
		break;
		default:
		{
			App->ren->Blit(playerTex, collider->x-separation, collider->y, idle_right.GetCurrentFrame(), 0);
			idle_right.NextFrame();
		}
		break;
		}

		if (current_spell != NONE_UNLOCKED)
			spells[current_spell]->Render();


	}
	return true;
}

bool Player::ShouldDraw()
{
	if(!is_invincible)
		return true;

	if(invin_draw_timer.Read()>current_draw_interval)
	{
		//base + (total-base)*percent/100
		current_draw_interval = min_invin_interval + (max_invin_interval - min_invin_interval)*((100-GetInvincibilityPercent()) / 100);
		invin_draw_timer.Reset();
		is_drawing = !is_drawing;
	}

	return is_drawing;
}


void Player::Respawn() {
	//ADD BLACKSCREEN
	if (!respawn_player && App->cam->GetCoveragePercent() < 100)
	{
		App->cam->CoverScreen(500, 200, 0, 0, 0);
		respawn_player = true;
		App->cam->CameraShake(15, 200);
	}
}


Player::~Player()
{
	delete spells[FIRE];
	delete spells[WIND];
	delete spells[WATER];
	delete spells[GROUND];
	delete spells[GRASS];
	App->cam->target = nullptr;

}