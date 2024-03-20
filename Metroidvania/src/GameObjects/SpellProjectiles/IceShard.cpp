#include "IceShard.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Particles.h"
IceShard::IceShard()
{
	ice_shard_right.AddFrame({ 162,134,48,24 });
	ice_shard_left.AddFrame({ 102,134,48,24 });
}

void IceShard::Init()
{
	particles = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/particles.png");
	spells = Engine->GetModule<Textures>().Load_Texture("Assets/Sprites/spells.png");
	ice_shard_left.mTexture = spells;
	ice_shard_right.mTexture = spells;

	r4ice = { 36,12,12,12 };
	ice.area_in_texture.push_back(&r4ice);
	ice.name = "ice";
	ice.minmax_speed_y = std::make_pair(-0.2, 0.2);
	ice.minmax_speed_x = std::make_pair(-0.4, 0.4);
	ice.minmax_lifespan = std::make_pair(75, 150);
	ice.minmax_x_offset = std::make_pair(22, 42);
	ice.minmax_y_offset = std::make_pair(4, 20);
	ice.minmax_frequency = std::make_pair(5, 20);
	ice.texture_name = particles;

	p = Engine->GetModule<Particles>().AddParticleEmitter(&ice, collider->x, collider->y);
}

bool IceShard::Loop(float dt)
{
	bool ret = true;

	collider->x += direction * speed;

	p->position_x = collider->x;
	p->position_y = collider->y;

	std::vector<RXRect*> colliders;
	Engine->GetModule<ObjectManager>().GetNearbyWalls(collider->x + collider->w / 2, collider->y + collider->h / 2, 50, colliders);

	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], collider, &result) == true)// he goin crash!
		{
			Engine->GetModule<ObjectManager>().DeleteObject(this);
			//Engine->GetModule<Particles>().to_delete.push_back(p);
			Engine->GetModule<Particles>().RemoveParticleEmitter(p);
			Engine->GetModule<Particles>().AddParticleEmitter(&ice, collider->x-8, collider->y, 300);
		}
	}

	return ret;
}

bool IceShard::Render()
{
	if (direction == 1)
	{
		Engine->GetModule<::Render>().RenderAnimation(ice_shard_right, collider->x, collider->y, -2);
		ice_shard_right.NextFrame();
	}
	else
	{
		Engine->GetModule<::Render>().RenderAnimation(ice_shard_left, collider->x, collider->y, -2);
		ice_shard_left.NextFrame();
	}

	return true;
}

void IceShard::Fire(bool left_dir)
{
	if (left_dir)
		direction = 1;
	else
		direction = -1;
}