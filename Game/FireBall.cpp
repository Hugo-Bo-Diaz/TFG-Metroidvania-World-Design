#include "FireBall.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Particles.h"

FireBall::FireBall()
{
	fireball_big.AddFrame({0,0,48,48});
	fireball_small.AddFrame({16,16,16,16});
}

bool FireBall::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;
	if(is_big)
		App->ren->Blit(App->tex->Get_Texture("fireball"), collider->x, collider->y, fireball_big.GetCurrentFrame(), -2);
	else
		App->ren->Blit(App->tex->Get_Texture("fireball"), collider->x, collider->y, fireball_small.GetCurrentFrame(), -2);

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->par->AddParticleEmitter(&App->par->fire,collider->x,collider->y,10000);
			App->phy->DeleteObject(this);
		}
	}

	return ret;
}

void FireBall::Fire(bool left_dir, bool _is_big)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;

	is_big = _is_big;
	
	if (!is_big)
	{
		collider->y += 16;
		collider->w = 16;
		collider->h = 16;

		nextpos->y += 16;
		nextpos->w = 16;
		nextpos->h = 16;
	}
}

FireBall::~FireBall()
{
}