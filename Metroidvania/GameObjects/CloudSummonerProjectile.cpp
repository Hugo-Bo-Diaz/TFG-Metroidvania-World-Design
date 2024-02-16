#include "CloudSummonerProjectile.h"
#include "Application.h"
#include "Input.h"
#include "Render.h"
#include "Particles.h"
#include "Audio.h"

#include "Player.h"
#include "Camera.h"


CloudSummonerProjectile::CloudSummonerProjectile()
{
	cloud_summoner = App->tex->Load_Texture("Assets/Sprites/enemies/cloud_summoner.png");
	particles = App->tex->Load_Texture("Assets/Sprites/particles.png");

	r13magic = { 0,36,12,12 };
	magic.area_in_texture.push_back(&r13magic);
	magic.name = "magic";
	magic.minmax_x_offset = std::make_pair(-5, 53);
	magic.minmax_y_offset = std::make_pair(24, 55);
	magic.minmax_speed_y = std::make_pair(-0.3, -0.5);
	magic.minmax_scale = std::make_pair(0.5, 1);
	magic.minmax_angle_speed = std::make_pair(5, 15);
	magic.minmax_angle = std::make_pair(0, 360);
	magic.minmax_lifespan = std::make_pair(300, 400);
	magic.minmax_frequency = std::make_pair(20, 40);
	magic.texture_name = particles;

	App->cam->CameraShake(7, 40);
	projectile.AddFrame({ 230,22,26,26 });
}

CloudSummonerProjectile::~CloudSummonerProjectile()
{
	App->par->AddParticleEmitter(&magic, collider->x, collider->y, 300);
}

bool CloudSummonerProjectile::Loop(float dt)
{
	bool ret = true;

	collider->x += speed_x;
	collider->y += speed_y;

	nextpos->x += speed_x;
	nextpos->y += speed_y;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], nextpos, &result) == SDL_TRUE)// he goin crash!
		{
			App->phy->DeleteObject(this);
			App->aud->PlaySFX(SFX_GROUND_HIT);
			App->cam->CameraShake(7, 40);
		}
	}

	return ret;
}

bool CloudSummonerProjectile::Render()
{
		App->ren->Blit(cloud_summoner, collider->x, collider->y, projectile.GetCurrentFrame(), -2);

	return true;
}

void CloudSummonerProjectile::Fire(float _speed_x, float _speed_y)
{
	speed_x = _speed_x;
	speed_y = _speed_y;

}
