#include "WindSlash.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"

WindSlash::WindSlash()
{
	windslash.AddFrame({ 0,222,48,48 });
	windslash.AddFrame({ 48,222,48,48 });
	windslash.AddFrame({ 96,222,48,48 });
	windslash.AddFrame({ 144,222,48,48 });

	p = App->par->AddParticleEmitter(&App->par->windslash,collider->x,collider->h);
}

bool WindSlash::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;
	windslash.NextFrame();

	p->position_x = collider->x;
	p->position_y = collider->y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			//App->par->to_delete.push_back(p);
			App->par->RemoveParticleEmitter(p);
			App->par->AddParticleEmitter(&App->par->windslash, collider->x, collider->y, 500);
		}
	}

	return ret;
}

bool WindSlash::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spells"), collider->x, collider->y, windslash.GetCurrentFrame(), -2);
	return true;
}

void WindSlash::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}