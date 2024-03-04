#include "CloudTrampoline.h"
#include "Application.h"
#include "Modules/Render.h"
#include "../Player.h"
#include "Modules/ProgressTracker.h"
#include "Modules/Textures.h"

CloudTrampoline::CloudTrampoline()
{
	cloudLeft = { 48,96,48,48 };
	cloudLoop = { 96,96,48,48 };
	cloudRight = { 144,96,48,48 };
}

void CloudTrampoline::Init()
{
	hazards = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/hazards.png");
}

bool CloudTrampoline::Loop(float dt)
{
	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
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


	Engine->GetModule<::Render>().Blit(hazards, collider->x , collider->y, &cloudLeft, -20);

	//top
	for (int i = 1; i < tilesx-1; ++i)
	{
		Engine->GetModule<::Render>().Blit(hazards, collider->x + 48 * i, collider->y, &cloudLoop, -20);
	}


	Engine->GetModule<::Render>().Blit(hazards, collider->x + collider->w - 48 , collider->y, &cloudRight, -20);

	return true;
}
