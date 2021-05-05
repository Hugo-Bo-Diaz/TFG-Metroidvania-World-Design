#include "GroundedElemental.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Particles.h"

GroundedElemental::GroundedElemental()
{

}

GroundedElemental::~GroundedElemental()
{
	//ADD PARTICLES
	App->par->AddParticleEmitter(&App->par->explosion, collider->x, collider->y, 600);

}

bool GroundedElemental::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2
	speed_y += acceleration_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

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
			if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE && collider->y < colliders[i]->y)// he goin crash!
			{
				speed_y = 0;
				nextpos->y -= result.h;
				
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
	if (change_direction||!floor_below)
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
		App->ren->Blit(App->tex->Get_Texture("groundelemental"), collider->x, collider->y, walking_left.GetCurrentFrame(), 0);
	else
		App->ren->Blit(App->tex->Get_Texture("groundelemental"), collider->x, collider->y, walking_right.GetCurrentFrame(), 0);

	return true;
}

void GroundedElemental::RecieveDamage(int dmg, int direction)
{
	health -= dmg;
	if (health <= 0)
	{
		App->phy->DeleteObject(this);
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
