#include "Shockwave.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"

Shockwave::Shockwave()
{
	shockwave_left.AddFrame({0,0,32,32});
	shockwave_right.AddFrame({32,0,32,32});
}

bool Shockwave::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;
	nextpos->x += x_speed;

	floor_check.x = x_speed + nextpos->x + nextpos->w/2;

	if (x_speed > 0)
	{
		App->ren->Blit(App->tex->Get_Texture("earthquake"), collider->x, collider->y, shockwave_right.GetCurrentFrame(), -2);
	}
	else
	{
		App->ren->Blit(App->tex->Get_Texture("earthquake"), collider->x, collider->y, shockwave_left.GetCurrentFrame(), -2);
	}

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;


		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE || SDL_PointInRect(&floor_check, colliders[i]) == SDL_FALSE)// he goin crash!
		{
			App->phy->DeleteObject(this);
		}
	}

	return ret;
}

void Shockwave::Fire(bool left_dir, float speed)
{
	//angle = _angle;

	x_speed = speed;

	if (!left_dir)
	{
		x_speed = -x_speed;
	}

	floor_check.y = nextpos->y + nextpos->h + 8;
}

Shockwave::~Shockwave()
{

}