#include "FlyingShield.h"
#include "Particles.h"
#include "Application.h"
#include "Audio.h"

FlyingShield::FlyingShield()
{
	flying_left.AddFrame({ 0,0,48,48 });

	flying_right.AddFrame({ 48,0,48,48 });

}

FlyingShield::FlyingShield(float _initial_y)
{
	initial_y = _initial_y;
}


FlyingShield::~FlyingShield()
{
	App->par->AddParticleEmitter(&App->par->explosion, collider->x, collider->y, 300);
}



bool FlyingShield::Loop(float dt)
{
	//STEP 1
	collider->x = nextpos->x;
	collider->y = nextpos->y;

	//STEP 2

	nextpos->x += speed_x;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 100, colliders);

	//IF IT HAS BEEN IN THIS STATE FOR MORE THAN 3 SECS GO BACKWARDS
	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)
		{
			if (result.h < result.w)
			{

				if (collider->y < colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y -= result.h;
				}

				if (collider->y > colliders[i]->y)// he goin crash!
				{
					//speed_y = 0;
					nextpos->y += result.h;
				}
			}
			else
			{
				if (collider->x > colliders[i]->x)
				{
					nextpos->x += result.w;
				}

				if (collider->x < colliders[i]->x)
				{
					nextpos->x -= result.w;
				}
			}
		}

	}

	//App->ren->DrawRect(&aggro, 0, 255, 0, 50,true);

	current_distance += speed_x;
	if (abs(current_distance) > patrol_width)
	{
		speed_x = -speed_x;
		current_distance = 0;
	}


	return true;
}

bool FlyingShield::Render()
{

	if (speed_x < 0)
	{
		App->ren->Blit(App->tex->Get_Texture("floating_shield"), collider->x, collider->y, flying_left.GetCurrentFrame(), 0);
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("floating_shield"), collider->x, collider->y, flying_right.GetCurrentFrame(), 0);
	}
	return true;
}

void FlyingShield::RecieveDamage(int dmg, int direction)
{
	if (direction == speed_x / abs(speed_x))
	{
		App->aud->PlaySFX(SFX_ENEMY_HIT);
		health -= dmg;
		if (health <= 0)
		{
			App->phy->DeleteObject(this);
		}
	}
	else
	{
		App->aud->PlaySFX(SFX_ENEMY_PING);
	}
}
