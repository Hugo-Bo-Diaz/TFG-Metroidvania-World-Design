#include "ShieldMonster.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Particles.h"
#include "Audio.h"
#include "Player.h"

ShieldMonster::ShieldMonster()
{

	right.AddFrame({ 0,0,112,140 });
	right.AddFrame({ 112,0,112,140 });
	right.AddFrame({ 0,140,112,140 });
	right.AddFrame({ 112,140,112,140 });

	left.AddFrame({ 224,0,112,140 });
	left.AddFrame({ 336,0,112,140 });
	left.AddFrame({ 224,140,112,140 });
	left.AddFrame({ 336,140,112,140 });

	arm_left = {112,0,112,128};
	arm_right = {0,0,112,128};

	range.w = 300;
	range.h = 50;

	aggro.w = 400;
	aggro.h = 144;

	shield.w = 50;
	shield.h = max_shield_height;

	HitBox.w = 100;
	HitBox.h = 50;
}

ShieldMonster::~ShieldMonster()
{
	//ADD PARTICLES
	App->par->AddParticleEmitter(&App->par->shield_monster_death, collider->x, collider->y, 400);

}

bool ShieldMonster::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_y += acceleration_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	aggro.x = collider->x + collider->w / 2 - aggro.w / 2;
	aggro.y = collider->y + collider->h - aggro.h;

	range.x = collider->x + collider->w / 2 - range.w / 2;
	range.y = collider->y + collider->h - range.h;

	if (gdirection > 0)
		shield.x = collider->x+collider->w;
	else
		shield.x = collider->x-shield.w;

	shield.y = collider->y;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(&aggro, collisions);

	isplayernearby = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER)
			{
				isplayernearby = true;
				target = (*it)->object;
			}
		}
	}

	App->phy->ClearCollisionArray(collisions);

	App->phy->GetCollisions(&range, collisions);

	isplayeronrange = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER)
			{
				isplayeronrange = true;
			}
		}
	}

	App->phy->ClearCollisionArray(collisions);

	App->phy->GetCollisions(&shield, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == FIRE_BALL)
			{
				App->aud->PlaySFX(SFX_ENEMY_PING);
				App->phy->DeleteObject((*it)->object);
				App->par->AddParticleEmitter(&App->par->metal, (*it)->object->collider->x, (*it)->object->collider->y, 300);
			}
			if ((*it)->type == ROCK)
			{
				App->aud->PlaySFX(SFX_ENEMY_PING);
				App->phy->DeleteObject((*it)->object);
				App->par->AddParticleEmitter(&App->par->metal, (*it)->object->collider->x, (*it)->object->collider->y, 300);
			}
			if ((*it)->type == PLAYER)
			{
				int direction = 0;
				int distance = (*it)->object->collider->x - collider->x;
				if (distance > 0)
					direction = 1;
				else
					direction = -1;

				((Player*)((*it)->object))->AddHealth(-1, direction);
			}
		}
	}
	App->phy->ClearCollisionArray(collisions);

	//arm_angle += 1;

	switch (current_state)
	{
	case ShieldMonster_PATROL:
	{
		if (isplayernearby)
		{
			current_state = ShieldMonster_CHASE;
		}

		distance_traveled += speed_x;

		if (speed_x > 0)
			gdirection = 1;
		else
			gdirection = -1;

		if (abs(distance_traveled) > patrol_range)
		{
			speed_x = -speed_x;
			distance_traveled = 0;
		}

	}
	break;
	case ShieldMonster_CHASE:
	{


		int direction = 0;
		int distance = target->collider->x - collider->x;
		if (distance > 0)
			direction = 1;
		else
			direction = -1;

		speed_x = direction * chase_speed;
		gdirection = direction;

		if (!isplayernearby)
		{
			current_state = ShieldMonster_PATROL;
			speed_x = direction * partrol_speed;
			target = nullptr;
		}

		if (isplayeronrange)
		{
			current_state = ShieldMonster_WINDUP;
			speed_x = 0;
			windupTimer.Reset();
		}
	}
	break;
	case ShieldMonster_WINDUP:
	{
		int dir = -gdirection;
		arm_angle = dir * (windupTimer.Read() / winduptime)*max_windupangle;

		if(gdirection<0)
			shield.h = (1 - sin((M_PI / 180)* arm_angle))*max_shield_height;
		else
			shield.h = (1 + sin((M_PI / 180)* arm_angle))*max_shield_height;


		if (windupTimer.Read() > winduptime)
		{
			current_state = ShieldMonster_ATTACK;
			attackTimer.Reset();
			
			hashitground = false;
		}
	}
	break;
	case ShieldMonster_ATTACK:
	{
		int dir = -gdirection;

		shield.h = sin((M_PI / 180)* arm_angle)*max_shield_height;

		if (gdirection < 0)//facing_left
		{
			arm_angle = +(max_windupangle - (dir * (attackTimer.Read() / attacktime)*max_windupangle));
			if (arm_angle < 0)
				arm_angle = 0;

			shield.h =(1- sin((M_PI / 180)* arm_angle))*max_shield_height;

		}
		else//facing_right
		{
			arm_angle = -180 + (max_windupangle - (dir * (attackTimer.Read() / attacktime)*max_windupangle));
			if (arm_angle > 0)
				arm_angle = 0;

			shield.h = (1+ sin((M_PI / 180)* arm_angle))*max_shield_height;
			printf("%d ", shield.h);

		}
		
		if (attackTimer.Read() > attacktime+50 && hashitground == false)
		{
			hashitground = true;

			if (gdirection < 0)//facing_left
			{
				HitBox.x = collider->x-HitBox.w;
			}
			else
			{
				HitBox.x = collider->x + collider->w;
			}

			HitBox.y = collider->y + collider->h - HitBox.h;

			App->phy->GetCollisions(&HitBox, collisions);

			int xemitter=HitBox.x;
			if (gdirection == 1)
				xemitter -= HitBox.w;
			else
				xemitter += HitBox.w;


			App->par->AddParticleEmitter(&App->par->shield_monster_hit, HitBox.x+HitBox.w/2, HitBox.y, 300);

			for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
			{
				if ((*it)->object != this)
				{
					if ((*it)->type == PLAYER)
					{
						((Player*)((*it)->object))->AddHealth(-1, gdirection);
					}
				}
			}
			App->phy->ClearCollisionArray(collisions);
		}

		
		if (attackTimer.Read() > totalanimation)
		{
			current_state = ShieldMonster_PATROL;
			speed_x = gdirection * partrol_speed;
			shield.h = max_shield_height;
		}

	}
	break;
	default:
		break;
	}


	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if (speed_y >acceleration_y)
	{
		floor_below = true;
	}

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Point p1;//left(should not collide)
		SDL_Point p2;//down(should collide)
		if (speed_x < 0)//left
		{
			p1.x = collider->x - abs(speed_x);
			p1.y = collider->y + collider->h / 2;

			p2.x = collider->x - abs(speed_x);
			p2.y = collider->y + collider->h + 10;

		}
		else if (speed_x > 0)
		{
			p1.x = collider->x + collider->w + abs(speed_x);
			p1.y = collider->y + collider->h / 2;

			p2.x = collider->x + collider->w + abs(speed_x);
			p2.y = collider->y + collider->h + 10;
		}

		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
		{
			if (result.h < result.w)
			{
				speed_y = 0;
				nextpos->y -= result.h;

				if (knocked_up)
				{
					speed_x = partrol_speed;
					knocked_up = false;
				}
			}
		}

		if (SDL_PointInRect(&p2, colliders[i]) == SDL_TRUE)
		{
			floor_below = true;
		}

		if (SDL_PointInRect(&p1, colliders[i]) == SDL_TRUE)
		{
			change_direction = true;
		}

	}
	if ((change_direction || !floor_below) && (!knocked_up || change_direction))
	{
		speed_x = -speed_x;
	}

	return true;
}

bool ShieldMonster::Render()
{

	if (App->debug)
	{
		App->ren->DrawRect(&aggro, 255, 255, 0, 100, true);
		App->ren->DrawRect(&range, 255, 255, 0, 100, true);
		App->ren->DrawRect(&shield, 255, 0, 0, 100, true);
	}

	if ((animation_timer.Read() > animation_interval_chase && current_state == ShieldMonster_CHASE) || (animation_timer.Read() > animation_interval_patrol && current_state == ShieldMonster_PATROL ))
	{
		if (gdirection < 0)
			left.NextFrame();
		else
			right.NextFrame();

		animation_timer.Reset();
	}
	int offset_y = 0;


	if (gdirection < 0)
	{
		App->ren->Blit(App->tex->Get_Texture("shield_monster"), collider->x, collider->y, left.GetCurrentFrame(), 0);
		if (left.current_frame+1 > left.amount_of_frames / 2)
		{
			offset_y += 10;
		}
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("shield_monster"), collider->x, collider->y, right.GetCurrentFrame(), 0);
		if (right.current_frame+1 > right.amount_of_frames / 2)
		{
			offset_y += 10;
		}
	}



	if (gdirection < 0)
		App->ren->Blit(App->tex->Get_Texture("shield_monster_arm"), collider->x-50, collider->y+8+offset_y, &arm_left, 0,arm_angle,1,1,98,16);
	else
		App->ren->Blit(App->tex->Get_Texture("shield_monster_arm"), collider->x+42, collider->y+8+offset_y, &arm_right, 0,arm_angle,1,1,14,16);

	return true;
}

void ShieldMonster::RecieveDamage(int dmg, int direction)
{

	int dir = -gdirection;
	if (direction == dir)
	{
		App->aud->PlaySFX(SFX_ENEMY_HIT);

		health -= dmg;
		if (health <= 0)
		{
			App->phy->DeleteObject(this);
		}
		else
		{
			float fromdir = ((gdirection*0.25) + 0.75);
			if (gdirection == 1)
				fromdir = 0.75;
			else
				fromdir = 0.25;

			App->par->AddParticleEmitter(&App->par->stone_death, collider->x+(collider->w*fromdir), collider->y+collider->h/2, 300);

		}
	}
	else
	{
		App->aud->PlaySFX(SFX_ENEMY_PING);

		float fromdir = ((gdirection*0.25) + 0.75);
		if (dir == 1)
			fromdir = 0.75;
		else
			fromdir = 0.25;

		App->par->AddParticleEmitter(&App->par->metal, collider->x + (collider->w*fromdir), collider->y + collider->h / 2, 150);
	}
}