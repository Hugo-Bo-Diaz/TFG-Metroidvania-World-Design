#include "GroundedElemental.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"

GroundedElemental::GroundedElemental()
{
}

GroundedElemental::GroundedElemental(std::list<ObjectProperty*>& aProperties)
{
	new (this) GroundedElemental;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("color") == 0)
		{
			c = (GroundedElementalColor)(int)(*it)->num_value;
		}
	}
}

void GroundedElemental::Destroy()
{
	//ADD PARTICLES
	switch (c)
	{
	case RED_GROUNDELEMENTAL:
		Engine->GetModule<Particles>().AddParticleEmitter(&fire_ge_death, collider->x, collider->y, 200);
		break;
	case BROWN_GROUNDEDELEMENTAL:
		Engine->GetModule<Particles>().AddParticleEmitter(&stone_death, collider->x, collider->y, 200);
		break;
	default:
		break;
	}

}

void GroundedElemental::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	groundelemental = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/groundelemental.png");

	mSFXHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/enemy_hit.wav");

	r14firegedeath = { 24,36,12,12 };
	r15firegedeath = { 36,36,12,12 };
	r16sandfirst = { 48,0,12,12 };
	r17sandsecond = { 48,12,12,12 };

	fire_ge_death.area_in_texture.push_back(&r14firegedeath);
	fire_ge_death.area_in_texture.push_back(&r15firegedeath);
	fire_ge_death.name = "fire grounded death";
	fire_ge_death.minmax_x_offset = std::make_pair(0, 48);
	fire_ge_death.minmax_y_offset = std::make_pair(0, 48);
	fire_ge_death.minmax_speed_x = std::make_pair(-0.6, 0.6);
	fire_ge_death.minmax_speed_y = std::make_pair(-2, -3);
	fire_ge_death.minmax_scale = std::make_pair(1, 1.5);
	fire_ge_death.minmax_angle = std::make_pair(0, 360);
	fire_ge_death.minmax_lifespan = std::make_pair(200, 500);
	fire_ge_death.minmax_frequency = std::make_pair(10, 20);
	fire_ge_death.minmax_acc_y = std::make_pair(0.1, 0.3);
	fire_ge_death.texture_name = particles;

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
}

bool GroundedElemental::Loop(float dt)
{
	//STEP 1
	collider->x += speed_x;
	collider->y += speed_y;

	//STEP 2
	speed_y += acceleration_y;

	std::vector<SDL_Rect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	bool change_direction = false;
	bool floor_below = false;
	if(speed_y >acceleration_y)
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
			if (SDL_IntersectRect(colliders[i], collider, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
			{
				speed_y = 0;
				collider -= result.h;
				
				if (knocked_up)
				{
					speed_x = patrol_speed;
					knocked_up = false;
				}
			}

			if (SDL_PointInRect(&p2, colliders[i]) == SDL_TRUE)
			{
				floor_below = true;
			}

			if (SDL_PointInRect(&p1, colliders[i]) == SDL_TRUE)
			{
				change_direction=true;
			}
		
	}
	if ((change_direction||!floor_below ) && (!knocked_up || change_direction))
	{
		speed_x = -speed_x;
	}

	return true;
}

bool GroundedElemental::Render()
{
	if (walking_left.amount_of_frames == 0 || walking_right.amount_of_frames == 0)
		SetAnimations(RED_GROUNDELEMENTAL);

	if (animation_timer.Read() > animation_frame_interval)
	{
		if (speed_x < 0)
			walking_left.NextFrame();
		else
			walking_right.NextFrame();

		animation_timer.Reset();
	}
	

	if (speed_x < 0)
		Engine->GetModule<::Render>().Blit(groundelemental, collider->x, collider->y, walking_left.GetCurrentFrame(), 0);
	else
		Engine->GetModule<::Render>().Blit(groundelemental, collider->x, collider->y, walking_right.GetCurrentFrame(), 0);

	return true;
}

void GroundedElemental::RecieveDamage(int dmg, int direction)
{
	Engine->GetModule<Audio>().PlaySFX(mSFXHit);
	health -= dmg;
	if (health <= 0)
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
	}
	speed_x = direction * 6;
	speed_y = -10;
	knocked_up = true;
}

void GroundedElemental::SetAnimations(GroundedElementalColor _c)
{
	c = _c;

	if (walking_left.amount_of_frames == 0)
	{
		walking_left.AddFrame({ 0  ,c * 48,48,48 });
		walking_left.AddFrame({ 48 ,c * 48,48,48 });
		walking_left.AddFrame({ 96 ,c * 48,48,48 });
		walking_left.AddFrame({ 144,c * 48,48,48 });
	}
	if (walking_right.amount_of_frames == 0)
	{
		walking_right.AddFrame({ 192,c * 48,48,48 });
		walking_right.AddFrame({ 240,c * 48,48,48 });
		walking_right.AddFrame({ 288,c * 48,48,48 });
		walking_right.AddFrame({ 336,c * 48,48,48 });
	}
}