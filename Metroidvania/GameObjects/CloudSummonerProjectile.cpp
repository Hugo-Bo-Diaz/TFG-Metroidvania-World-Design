#include "CloudSummonerProjectile.h"
#include "Application.h"
#include "Input.h"
#include "Console.h"
#include "Render.h"
#include "Particles.h"
#include "Audio.h"

#include "Player.h"
#include "Camera.h"


CloudSummonerProjectile::CloudSummonerProjectile()
{
	App->cam->CameraShake(7, 40);
	projectile.AddFrame({ 230,22,26,26 });
}

CloudSummonerProjectile::~CloudSummonerProjectile()
{
	App->par->AddParticleEmitter(&App->par->magic, collider->x, collider->y, 300);
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
		App->ren->Blit(App->tex->Get_Texture("cloud_summoner"), collider->x, collider->y, projectile.GetCurrentFrame(), -2);

	return true;
}

void CloudSummonerProjectile::Fire(float _speed_x, float _speed_y)
{
	speed_x = _speed_x;
	speed_y = _speed_y;

}
