#include "ShieldMonster.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"
#include "../Player.h"
#include "Modules/Debug.h"
#include "../SpellProjectiles/FireBall.h"
#include "../SpellProjectiles/Rock.h"

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

	arm_left.AddFrame({112,0,112,128});
	arm_right.AddFrame({0,0,112,128});

	range.w = 300;
	range.h = 50;

	aggro.w = 400;
	aggro.h = 144;

	shield.w = 50;
	shield.h = max_shield_height;

	HitBox.w = 100;
	HitBox.h = 50;
}

void ShieldMonster::Destroy()
{
	//ADD PARTICLES
	Engine->GetModule<Particles>().AddParticleEmitter(&shield_monster_death, collider->x, collider->y, 400);

}

void ShieldMonster::Init()
{
	nextpos = new RXRect();
	nextpos->x = collider->x;
	nextpos->y = collider->y;
	nextpos->w = collider->w;
	nextpos->h = collider->h;

	shield_monster = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/shield_monster.png");
	shield_monster_arm = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/shield_monster_arm.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");
	mSFXPing = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/ping.wav");

	right.mTexture = shield_monster;
	left.mTexture = shield_monster;
	arm_left.mTexture = shield_monster_arm;
	arm_right.mTexture = shield_monster_arm;


	r8ground = { 0,24,12,12 };
	r9ground = { 12,24,12,12 };
	r16sandfirst = { 48,0,12,12 };
	r17sandsecond = { 48,12,12,12 };
	r18metalfirst = { 48,24,12,12 };
	r19metalsecond = { 48,36,12,12 };

	shield_monster_hit.area_in_texture.push_back(&r16sandfirst);
	shield_monster_hit.area_in_texture.push_back(&r9ground);
	shield_monster_hit.area_in_texture.push_back(&r8ground);
	shield_monster_hit.name = "shield_monster_floor_hit";
	shield_monster_hit.minmax_x_offset = std::make_pair(-30, 30);
	shield_monster_hit.minmax_y_offset = std::make_pair(-30, 30);
	shield_monster_hit.minmax_lifespan = std::make_pair(75, 150);
	shield_monster_hit.minmax_angle_speed = std::make_pair(5, 15);
	shield_monster_hit.minmax_angle = std::make_pair(0, 360);
	shield_monster_hit.minmax_acc_y = std::make_pair(1, 2);
	shield_monster_hit.minmax_frequency = std::make_pair(10, 15);
	shield_monster_hit.texture_name = particles;

	shield_monster_death.area_in_texture.push_back(&r16sandfirst);
	shield_monster_death.area_in_texture.push_back(&r17sandsecond);
	shield_monster_death.area_in_texture.push_back(&r9ground);
	shield_monster_death.area_in_texture.push_back(&r8ground);
	shield_monster_death.name = "stone_monster_death";
	shield_monster_death.minmax_x_offset = std::make_pair(0, 112);
	shield_monster_death.minmax_y_offset = std::make_pair(0, 140);
	shield_monster_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	shield_monster_death.minmax_speed_y = std::make_pair(-2, -3);
	shield_monster_death.minmax_scale = std::make_pair(1, 1.5);
	shield_monster_death.minmax_angle = std::make_pair(0, 360);
	shield_monster_death.minmax_lifespan = std::make_pair(200, 500);
	shield_monster_death.minmax_frequency = std::make_pair(7, 15);
	shield_monster_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	shield_monster_death.texture_name = particles;

	stone_death.area_in_texture.push_back(&r16sandfirst);
	stone_death.area_in_texture.push_back(&r17sandsecond);
	stone_death.name = "stone_monster_death";
	stone_death.minmax_x_offset = std::make_pair(0, 48);
	stone_death.minmax_y_offset = std::make_pair(0, 48);
	stone_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	stone_death.minmax_speed_y = std::make_pair(-2, -3);
	stone_death.minmax_scale = std::make_pair(1, 1.5);
	stone_death.minmax_angle = std::make_pair(0, 360);
	stone_death.minmax_lifespan = std::make_pair(200, 500);
	stone_death.minmax_frequency = std::make_pair(7, 15);
	stone_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	stone_death.texture_name = particles;

	metal.area_in_texture.push_back(&r18metalfirst);
	metal.area_in_texture.push_back(&r19metalsecond);
	metal.name = "metal";
	metal.minmax_speed_y = std::make_pair(-6, -4);
	metal.minmax_speed_x = std::make_pair(-3, 3);
	metal.minmax_lifespan = std::make_pair(75, 150);
	metal.minmax_angle_speed = std::make_pair(5, 15);
	metal.minmax_angle = std::make_pair(0, 360);
	metal.minmax_x_offset = std::make_pair(-15, 15);
	metal.minmax_y_offset = std::make_pair(-15, 15);
	metal.minmax_acc_y = std::make_pair(1, 2);
	metal.minmax_frequency = std::make_pair(8, 15);
	metal.texture_name = particles;


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
	Engine->GetModule<ObjectManager>().GetCollisions(&aggro, collisions);

	isplayernearby = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				isplayernearby = true;
				target = (*it)->object;
			}
		}
	}

	Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

	Engine->GetModule<ObjectManager>().GetCollisions(&range, collisions);

	isplayeronrange = false;
	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				isplayeronrange = true;
			}
		}
	}

	Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

	Engine->GetModule<ObjectManager>().GetCollisions(&shield, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<FireBall>())
			{
				Engine->GetModule<Audio>().PlaySFX(mSFXPing);
				Engine->GetModule<ObjectManager>().DeleteObject((*it)->object);
				Engine->GetModule<Particles>().AddParticleEmitter(&metal, (*it)->object->collider->x, (*it)->object->collider->y, 300);
			}
			if ((*it)->object->IsSameTypeAs<Rock>())
			{
				Engine->GetModule<Audio>().PlaySFX(mSFXPing);
				Engine->GetModule<ObjectManager>().DeleteObject((*it)->object);
				Engine->GetModule<Particles>().AddParticleEmitter(&metal, (*it)->object->collider->x, (*it)->object->collider->y, 300);
			}
			if ((*it)->object->IsSameTypeAs<Player>())
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
	Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);

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
			shield.h = (1 - sin((3.14 / 180)* arm_angle))*max_shield_height;
		else
			shield.h = (1 + sin((3.14 / 180)* arm_angle))*max_shield_height;


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

		shield.h = sin((3.14 / 180)* arm_angle)*max_shield_height;

		if (gdirection < 0)//facing_left
		{
			arm_angle = +(max_windupangle - (dir * (attackTimer.Read() / attacktime)*max_windupangle));
			if (arm_angle < 0)
				arm_angle = 0;

			shield.h =(1- sin((3.14 / 180)* arm_angle))*max_shield_height;

		}
		else//facing_right
		{
			arm_angle = -180 + (max_windupangle - (dir * (attackTimer.Read() / attacktime)*max_windupangle));
			if (arm_angle > 0)
				arm_angle = 0;

			shield.h = (1+ sin((3.14 / 180)* arm_angle))*max_shield_height;
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

			Engine->GetModule<ObjectManager>().GetCollisions(&HitBox, collisions);

			int xemitter=HitBox.x;
			if (gdirection == 1)
				xemitter -= HitBox.w;
			else
				xemitter += HitBox.w;


			Engine->GetModule<Particles>().AddParticleEmitter(&shield_monster_hit, HitBox.x+HitBox.w/2, HitBox.y, 300);

			for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
			{
				if ((*it)->object != this)
				{
					if ((*it)->object->IsSameTypeAs<Player>())
					{
						((Player*)((*it)->object))->AddHealth(-1, gdirection);
					}
				}
			}
			Engine->GetModule<ObjectManager>().ClearCollisionArray(collisions);
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


	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if (speed_y >acceleration_y)
	{
		floor_below = true;
	}

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXPoint p1;//left(should not collide)
		RXPoint p2;//down(should collide)
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

		RXRect result;
		if (RXRectCollision(colliders[i], nextpos, &result) == true && collider->y < colliders[i]->y)// he goin crash!
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

		if (RXPointInRect(&p2, colliders[i]) == true)
		{
			floor_below = true;
		}

		if (RXPointInRect(&p1, colliders[i]) == true)
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
		Engine->GetModule<::Render>().RenderAnimation(left, collider->x, collider->y);
		if (left.GetCurrentFrameNumber() +1 > left.GetAmountOfFrames() / 2)
		{
			offset_y += 10;
		}
	}
	else
	{
		Engine->GetModule<::Render>().RenderAnimation(right, collider->x, collider->y);
		if (right.GetCurrentFrameNumber()+1 > right.GetAmountOfFrames() / 2)
		{
			offset_y += 10;
		}
	}

	if (gdirection < 0)
		Engine->GetModule<::Render>().RenderAnimation(arm_left, collider->x-50, collider->y+8+offset_y, 0, RenderQueue::RENDER_GAME,arm_angle,1,1,98,16);
	else
		Engine->GetModule<::Render>().RenderAnimation(arm_right, collider->x+42, collider->y+8+offset_y, 0, RenderQueue::RENDER_GAME,arm_angle,1,1,14,16);

	return true;
}

void ShieldMonster::RenderDebug()
{
	Engine->GetModule<::Render>().RenderRect(aggro, RXColor{ 255, 255, 0, 100 }, true, RenderQueue::RENDER_DEBUG, 0);
	Engine->GetModule<::Render>().RenderRect(range, RXColor{ 255, 255, 0, 100 }, true, RenderQueue::RENDER_DEBUG, 0);
	Engine->GetModule<::Render>().RenderRect(shield, RXColor{ 255, 0, 0, 100 }, true, RenderQueue::RENDER_DEBUG, 0);

}


void ShieldMonster::RecieveDamage(int dmg, int direction)
{

	int dir = -gdirection;
	if (direction == dir)
	{
		Engine->GetModule<Audio>().PlaySFX(mSFXHit);

		health -= dmg;
		if (health <= 0)
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
		}
		else
		{
			float fromdir = ((gdirection*0.25) + 0.75);
			if (gdirection == 1)
				fromdir = 0.75;
			else
				fromdir = 0.25;

			Engine->GetModule<Particles>().AddParticleEmitter(&stone_death, collider->x+(collider->w*fromdir), collider->y+collider->h/2, 300);

		}
	}
	else
	{
		Engine->GetModule<Audio>().PlaySFX(mSFXPing);

		float fromdir = ((gdirection*0.25) + 0.75);
		if (dir == 1)
			fromdir = 0.75;
		else
			fromdir = 0.25;

		Engine->GetModule<Particles>().AddParticleEmitter(&metal, collider->x + (collider->w*fromdir), collider->y + collider->h / 2, 150);
	}
}