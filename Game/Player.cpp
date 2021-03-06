#include "Player.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Camera.h"
#include "SceneController.h"

#include "Fire.h"
#include "Wind.h"
#include "Water.h"
#include "Grass.h"
#include "Ground.h"

Player::Player()
{
	speed_y = -10;
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

	pos_x = App->con->AddVar_track("position x", &x);
	pos_y = App->con->AddVar_track("position y", &y);
	xspeed = App->con->AddVar_track("speed", &spd);

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


}

bool Player::Loop(float dt)
{

	bool ret = true;

	spd = speed_x;

	x = collider->x;
	y = collider->y;

	/*
	STEPS:
	STEP 1: collider to its next frame calculated position (calculated previous frame)
	STEP 2: calculate next frame position
	STEP 3: correct next frame position
	STEP 4: profit
	*/
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_x += acceleration_x;
	speed_y += acceleration_y;
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

	if (left)//if recieved msg for left then do this
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

	if (right)//if recieved msg for right then do this
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


	if (stop)
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
		speed_y = -12;
		grounded = false;
		acceleration_y = 0.5;
	}

	if (speed_y>1||(App->inp->GetInput(BUTTON_1) == BUTTON_RELEASE && grounded == false))
	{
		if (speed_y < -2)
		{
			speed_y = -2;
		}

		acceleration_y = 1.0;
	}

	//if ((App->inp->GetButton(B) == BUTTON_RELEASE))
	//{
	//	LockMovement(1000);
	//}

	//if ((App->inp->GetButton(Y) == BUTTON_RELEASE))
	//{
	//	UnlockMovement();
	//}


	//the habilities are handled here

	spells[current_spell]->Loop(dt);
	if (App->inp->GetInput(L_SHOULDER) == KEY_DOWN)
	{
		if (current_spell == FIRE)//loop to last
		{
			current_spell =GROUND;
		}
		else
		{
			current_spell = (spell_type)(current_spell - 1);
		}
	}
	if (App->inp->GetInput(R_SHOULDER) == KEY_DOWN)//START
	{
		if (current_spell == GROUND)//loop to FIRST
		{
			current_spell = FIRE;
		}
		else
		{
			current_spell = (spell_type)(current_spell + 1);
		}
	}

	//RENDER
	switch (current_state)
	{
	case IDLE:
	{
		if (is_right)
		{
			App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, idle_right.GetCurrentFrame(), 0);
			idle_right.NextFrame();
		}
		else
		{
			App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, idle_left.GetCurrentFrame(), 0);
			idle_left.NextFrame();
		}
	}
	break;
	case WALKING_LEFT:
	{
		App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, walking_left.GetCurrentFrame(), 0);
		walking_left.NextFrame();
	}
	break;
	case WALKING_RIGHT:
	{
		App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, walking_right.GetCurrentFrame(), 0);
		walking_right.NextFrame();
	}
	break;
	case AIRBORNE_LEFT:
	{
		App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, air_left.GetCurrentFrame(), 0);
		air_left.NextFrame();
	}
	break;
	case AIRBORNE_RIGHT:
	{
		App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, air_right.GetCurrentFrame(), 0);
		air_right.NextFrame();
	}
	break;
	default:
	{
		App->ren->Blit(App->tex->Get_Texture("player"), collider->x, collider->y, idle_right.GetCurrentFrame(), 0);
		idle_right.NextFrame();
	}
		break;
	}
	
	//trail
	for (int i = 29; i > 0; --i)
	{
		trail[i].x = trail[i-1].x;
		trail[i].y = trail[i-1].y;
	}

	trail[0].x = collider->x+collider->w/2;
	trail[0].y = collider->y+collider->h/2;

	App->ren->DrawTrail(trail, 30);


	//handling lock movement
	if (movement_is_locked && time_to_lock_movement != -1 && movement_lock.Read() > time_to_lock_movement)
	{
		UnlockMovement();
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

Player::~Player()
{
	App->con->DelVar_track(pos_x);
	App->con->DelVar_track(pos_y);
	App->con->DelVar_track(xspeed);
}