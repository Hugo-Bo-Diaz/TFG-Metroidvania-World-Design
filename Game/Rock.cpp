#include "Rock.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"

Rock::Rock()
{
	rock_sprite.AddFrame({ 0,0,32,32 });
}

bool Rock::Loop(float dt)
{
	bool ret = true;

	collider->x += x_speed;
	nextpos->x += x_speed;

	collider->y += y_speed;
	nextpos->y += y_speed;

	y_speed += gravity;

	App->ren->Blit(App->tex->Get_Texture("rock"), collider->x, collider->y, rock_sprite.GetCurrentFrame(), -2);

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

void Rock::Fire(bool left_dir, float angle, float speed, float _gravity)
{
	//angle = _angle;

	x_speed = speed * cos(angle*(3.1428 / 180));
	y_speed = -speed * sin(angle*(3.1428 / 180));
	
	if (!left_dir)
	{
		x_speed = -x_speed;
	}
	
	gravity = _gravity;
}

Rock::~Rock()
{

}