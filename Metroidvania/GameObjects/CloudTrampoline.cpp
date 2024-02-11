#include "CloudTrampoline.h"
#include "Application.h"
#include "Render.h"
#include "Player.h"
#include "ProgressTracker.h"
#include "Textures.h"
#include "EntityIDs.h"

CloudTrampoline::CloudTrampoline()
{
	cloudLeft = { 48,96,48,48 };
	cloudLoop = { 96,96,48,48 };
	cloudRight = { 144,96,48,48 };
}

bool CloudTrampoline::Loop(float dt)
{
	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->type == PLAYER_ID)
			{
				((Player*)(*it)->object)->speed_y = -30;
			}
		}
	}

	return true;
}

bool CloudTrampoline::Render()
{
	int tilesx = collider->w / 48;
	int tilesy = collider->h / 48;


	App->ren->Blit(App->tex->Get_Texture("hazards"), collider->x , collider->y, &cloudLeft, -20);

	//top
	for (int i = 1; i < tilesx-1; ++i)
	{
		App->ren->Blit(App->tex->Get_Texture("hazards"), collider->x + 48 * i, collider->y, &cloudLoop, -20);
	}


	App->ren->Blit(App->tex->Get_Texture("hazards"), collider->x + collider->w - 48 , collider->y, &cloudRight, -20);

	return true;
}
