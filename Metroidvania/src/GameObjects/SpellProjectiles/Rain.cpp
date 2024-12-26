#include "Rain.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Cloud.h"

Rain::Rain(EngineAPI* aAPI) :Engine(aAPI)
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/spells.png", spells);

}

void Rain::Loop()
{
	hitbox.y += speed;

	std::vector<RXRect*> colliders;
	Engine->GetModule<SceneController>().GetNearbyWalls(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2, 50, colliders);


	for (int i = 0; i < colliders.size(); ++i)
	{
		RXRect result;
		if (RXRectCollision(colliders[i], &hitbox, &result))// he goin crash!
		{
			parent->DeleteRainDrop(this);
		}
	}

}

void Rain::Render()
{
	Engine->GetModule<::Render>().RenderTexture(spells, hitbox.x, hitbox.y, blitrect, -2);
}
