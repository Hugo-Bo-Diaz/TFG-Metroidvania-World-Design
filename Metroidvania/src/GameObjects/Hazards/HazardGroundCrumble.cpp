#include "HazardGroundCrumble.h"
#include "Application.h"
#include "Modules/Render.h"
#include "../Player.h"
#include "Modules/ProgressTracker.h"


HazardGroundCrumble::HazardGroundCrumble()
{
	tex_rect = { 144,48,48,48 };
	rPart1 = { 24,0,12,14 };
	rPart2 = { 12,12,12,12 };
}

void HazardGroundCrumble::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/hazards.png", hazards);
	crumble_timer.Stop();
	wallId = Engine->GetModule<SceneController>().AddWall(collider);
	check_collider = { collider.x, collider.y - 16, collider.w, 16 };
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/particles.png", particles);


	particle_Crumble.area_in_texture.push_back(&rPart1);
	particle_Crumble.area_in_texture.push_back(&rPart2);
	particle_Crumble.name = "HazardCrumbleGround";
	particle_Crumble.minmax_x_offset = std::make_pair(0, 48);
	particle_Crumble.minmax_y_offset = std::make_pair(0, 40);
	particle_Crumble.minmax_lifespan = std::make_pair(500, 750);
	particle_Crumble.minmax_angle_speed = std::make_pair(3, 7);
	particle_Crumble.minmax_angle = std::make_pair(0, 180);
	particle_Crumble.minmax_acc_y = std::make_pair(0.5, 1);
	particle_Crumble.minmax_frequency = std::make_pair(2, 5);
	
	particle_Crumble.texture_name = particles;

}

bool HazardGroundCrumble::Loop(float dt)
{
	std::vector<collision> collisions;
	Engine->GetModule<SceneController>().GetCollisions(&check_collider, collisions);

	for (std::vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it).object != this && (*it).object->IsSameTypeAs<Player>())
		{
			if (((Player*)(*it).object)->collider.y < collider.y && !crumble_timer.isRunning())
			{
				crumble_timer.Start();
				((Player*)(*it).object)->speed_y = 0;
				Engine->GetModule<::Render>().AddParticleEmitter(&particle_Crumble, collider.x, collider.y, 80);
			}
		}
	}

	if (crumble_timer.Read() > crumble_time)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
	}

	return true;
}

bool HazardGroundCrumble::Render()
{
	Engine->GetModule<::Render>().RenderTexture(hazards, collider.x , collider.y, tex_rect, -20);
	return true;
}

void HazardGroundCrumble::Destroy()
{
	Engine->GetModule<SceneController>().DeleteWall(wallId);
}