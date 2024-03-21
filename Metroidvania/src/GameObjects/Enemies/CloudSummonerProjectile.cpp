#include "CloudSummonerProjectile.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Particles.h"
#include "Modules/Audio.h"

#include "../Player.h"
#include "Modules/Camera.h"


CloudSummonerProjectile::CloudSummonerProjectile()
{
}

void CloudSummonerProjectile::Destroy()
{
	Engine->GetModule<Particles>().AddParticleEmitter(&magic, collider.x, collider.y, 300);
}

void CloudSummonerProjectile::Init()
{
	cloud_summoner = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/enemies/cloud_summoner.png");
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");

	mSFXLand = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/hit_floor.wav");

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

	Engine->GetModule<Camera>().CameraShake(7, 40);
	projectile.AddFrame({ 230,22,26,26 });
	projectile.mTexture = cloud_summoner;
}

bool CloudSummonerProjectile::Loop(float dt)
{
	bool ret = true;

	collider.x += speed_x;
	collider.y += speed_y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider.x + collider.w / 2, collider.y + collider.h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], &collider, &result) == true)// he goin crash!
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
			Engine->GetModule<Audio>().PlaySFX(mSFXLand);
			Engine->GetModule<Camera>().CameraShake(7, 40);
		}
	}

	return ret;
}

bool CloudSummonerProjectile::Render()
{
		Engine->GetModule<::Render>().RenderAnimation(projectile, collider.x, collider.y, -2);

	return true;
}

void CloudSummonerProjectile::Fire(float _speed_x, float _speed_y)
{
	speed_x = _speed_x;
	speed_y = _speed_y;

}
