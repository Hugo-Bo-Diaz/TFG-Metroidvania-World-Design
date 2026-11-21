#include "HazardWater.h"
#include "Application.h"
#include "Modules/Render.h"
#include "../Player.h"
#include "Modules/ProgressTracker.h"
#include "../../SceneProcessing.h"

HazardWater::HazardWater()
{
	water = {48,144,48,48};
	water_top1 = {96,144,48,48};
	water_top2 = {144,144,48,48};
	water_top3 = {196,144,48,48};
	water_top4 = {240,144,48,48};
}

void HazardWater::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/hazards.png", hazards);
}

bool HazardWater::Loop(float dt)
{
	if (water_animation.Read() > iteration_time)
	{
		offset_cycle+=1;
		if (offset_cycle > 3)
		{
			offset_cycle = 0;
		}

		water_animation.Reset();
	}

	return true;
}

bool HazardWater::Render()
{
	int tilesx = collider.w/48;
	int tilesy = collider.h/48;

	//top
	int localoffset = 0;
	for (int i = 0; i < tilesx; ++i)
	{
		localoffset += 1;
		int locallocaloffset = localoffset + offset_cycle;
		locallocaloffset = locallocaloffset % 4;
		switch (locallocaloffset)
		{
		case 0:
			Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, collider.y, water_top1, -20);
			break;
		case 1:
			Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, collider.y, water_top2, -20);
			break;
		case 2:
			Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, collider.y, water_top3, -20);
			break;
		case 3:
			Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, collider.y, water_top4, -20);
			break;
		}
	}

	//body
	for (int i = 0; i < tilesx; ++i)
	{
		for (int j = 0; j < tilesy-1; ++j)
		{
			Engine->GetModule<::Render>().RenderTexture(hazards, collider.x + 48 * i, (collider.y + 48) + 48 * j, water, -20);
		}
	}

	return true;
}
