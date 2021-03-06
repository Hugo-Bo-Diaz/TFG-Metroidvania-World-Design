#include "WindSlash.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"

WindSlash::WindSlash()
{
	windslash.AddFrame({ 0,0,48,48 });
	windslash.AddFrame({ 48,0,48,48 });
	windslash.AddFrame({ 96,0,48,48 });
	windslash.AddFrame({ 144,0,48,48 });
}

bool WindSlash::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;
	nextpos->x += direction * speed;
	App->ren->Blit(App->tex->Get_Texture("windslash"), collider->x, collider->y, windslash.GetCurrentFrame(), -2);
	windslash.NextFrame();

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

void WindSlash::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}

WindSlash::~WindSlash()
{

}