#include "IceShard.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"

IceShard::IceShard()
{
	ice_shard_right.AddFrame({ 0,0,48,24 });
	ice_shard_left.AddFrame({ 48,0,48,24 });
}

bool IceShard::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;

	if (direction == 1)
	{
		App->ren->Blit(App->tex->Get_Texture("iceshard"), collider->x, collider->y, ice_shard_right.GetCurrentFrame(), -2);
		ice_shard_right.NextFrame();
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("iceshard"), collider->x, collider->y, ice_shard_left.GetCurrentFrame(), -2);
		ice_shard_left.NextFrame();
	}

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
		}
	}

	return ret;
}

void IceShard::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}

IceShard::~IceShard()
{

}