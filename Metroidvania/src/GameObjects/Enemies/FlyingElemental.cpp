#include "FlyingElemental.h"
#include "Modules/Particles.h"
#include "Application.h"
#include "Modules/Audio.h"
#include "../Player.h"

FlyingElemental::FlyingElemental()
{
}

FlyingElemental::FlyingElemental(std::list<ObjectProperty*>& aProperties)
{
	new (this) FlyingElemental;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("color") == 0)
		{
			c = (FlyingElementalColor)(int)(*it)->num_value;
		}
	}
}

FlyingElemental::FlyingElemental(float _initial_y)
{
	initial_y = _initial_y;
}


void FlyingElemental::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&explosion, collider.x, collider.y, 300);
}



void FlyingElemental::Init()
{
	initial_y = collider.y;

	nextpos = new RXRect();
	nextpos->x = collider.x;
	nextpos->y = collider.y;
	nextpos->w = collider.w;
	nextpos->h = collider.h;

	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	flyingelemental = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/flyingelemental.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r1exp = { 0,0,12,12 };
	explosion.area_in_texture.push_back(&r1exp);
	explosion.name = "explosion";
	explosion.minmax_x_offset = std::make_pair(-20, 20);
	explosion.minmax_y_offset = std::make_pair(-20, 20);
	explosion.minmax_speed_y = std::make_pair(-2, -3);
	explosion.minmax_speed_x = std::make_pair(-0.6, 0.6);
	explosion.minmax_scale_speed = std::make_pair(0.01, 0.02);
	explosion.minmax_lifespan = std::make_pair(200, 500);
	explosion.minmax_frequency = std::make_pair(10, 50);
	explosion.minmax_acc_y = std::make_pair(0.05, 0.2);
	explosion.texture_name = particles;
}

bool FlyingElemental::Loop(float dt)
{
	//STEP 1
	collider.x = nextpos->x;
	collider.y = nextpos->y;

	//STEP 2
	speed_y += acceleration_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 100, colliders);

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], nextpos, &result) == true)
		{
			if (result.h < result.w)
			{

				if (collider.y < colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y -= result.h;
				}

				if (collider.y > colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y += result.h;
					speed_y = 0;
				}
			}
			else
			{
				if (collider.x > colliders[i]->x)
				{
					nextpos->x += result.w;
				}

				if (collider.x < colliders[i]->x)
				{
					nextpos->x -= result.w;
				}
			}
		}

	}


	switch (state)
	{
	case FE_PATROL:
	{
		if (speed_x < 0)
		{
			aggro.x = collider.x - aggro.w;
			aggro.y = collider.y;
		}
		else
		{
			aggro.x = collider.x + collider.w;
			aggro.y = collider.y;
		}

		//detect player
		std::vector<collision*> collisions;
		Engine->GetModule<ObjectManager>().GetCollisions(&aggro, collisions);

		for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
		{
			if ((*it)->object != this)
			{
				if ((*it)->object->IsSameTypeAs<Player>())
				{
					state = FE_STARTING_CHARGE;
					starting.Reset();
					starting.Start();

				}
			}
		}

		//Engine->GetModule<::Render>().DrawRect(&aggro, 0, 255, 0, 50,true);

		current_distance += speed_x;

		if (abs(current_distance) > patrol_width)
		{
			speed_x = -speed_x;
			current_distance = 0;
		}

		if (collider.y < initial_y)
		{
			speed_y = bounce_strength;
		}

		last_state = FE_PATROL;
	}
		break;
	case FE_STARTING_CHARGE:
	{
		acceleration_y = 0;
		speed_y = 0;

		if (starting.Read() > time_starting)
		{
			state = FE_CHARGE;
		}

		if (last_state == FE_PATROL)
		{
			if (speed_x > 0)
			{
				speed_x = -speed_starting;
			}
			else
			{
				speed_x = speed_starting;
			}
		}
		last_state = FE_STARTING_CHARGE;
	}
		break;
	case FE_CHARGE:
	{
		if (last_state == FE_STARTING_CHARGE)
		{
			charged = false;
			speed_y = charge_speed_y;

			charge_timer.Reset();

			if (speed_x > 0)
				speed_x = -charge_speed_x;
			else
				speed_x = charge_speed_x;
		}
		acceleration_y = charge_accel_y;

		if (speed_y < 0)
		{
			charged = true;
		}

		if (charge_timer.Read() > 5000)
		{
			speed_x = -speed_x;
			charge_timer.Reset();
		}

		if(collider.y<initial_y && charged)
		{
			state = FE_PATROL;
			if(speed_x<0)
				speed_x = patrol_speed;
			else
				speed_x = -patrol_speed;
		}

		last_state = FE_CHARGE;
	}
		break;
	default:
		break;
	}

	return true;
}

bool FlyingElemental::Render()
{
	if (flying_left.GetAmountOfFrames() == 0 || flying_right.GetAmountOfFrames() == 0)
		SetAnimations(RED_FLYINGELEMENTAL);

	if (state == FE_PATROL || state == FE_CHARGE)
	{
		if (speed_x < 0)
		{
			flying_left.NextFrame();
			Engine->GetModule<::Render>().RenderAnimation(flying_left, collider.x, collider.y);
		}
		else
		{
			flying_right.NextFrame();
			Engine->GetModule<::Render>().RenderAnimation(flying_right, collider.x, collider.y);
		}
	}

	if (state == FE_STARTING_CHARGE)
	{
		if (speed_x > 0)
		{
			flying_left.NextFrame();
			Engine->GetModule<::Render>().RenderAnimation(flying_left, collider.x, collider.y);
		}
		else
		{
			flying_right.NextFrame();
			Engine->GetModule<::Render>().RenderAnimation(flying_right, collider.x, collider.y);
		}
	}

	return true;
}

void FlyingElemental::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}

	speed_x = -direction * speed_x;
	speed_y = -10;

}

void FlyingElemental::SetAnimations(FlyingElementalColor _c)
{
	c = _c;

	if (flying_left.GetAmountOfFrames() == 0)
	{
		flying_left.AddFrame({ 0  ,c * 56,56,56 });
		flying_left.AddFrame({ 56 ,c * 56,56,56 });
		flying_left.AddFrame({ 112,c * 56,56,56 });
		flying_left.AddFrame({ 168,c * 56,56,56 });
		flying_left = flyingelemental;
	}
	if (flying_right.GetAmountOfFrames() == 0)
	{
		flying_right.AddFrame({ 224,c * 56,56,56 });
		flying_right.AddFrame({ 280,c * 56,56,56 });
		flying_right.AddFrame({ 336,c * 56,56,56 });
		flying_right.AddFrame({ 392,c * 56,56,56 });
		flying_right = flyingelemental;
	}
}