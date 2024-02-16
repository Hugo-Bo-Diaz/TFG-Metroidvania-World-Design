#include "Rain.h"
#include "Application.h"
#include "Render.h"
#include "Cloud.h"
#include "Physics.h"

Rain::Rain()
{
	spells = App->tex->Load_Texture("Assets/Sprites/spells.png");

}

void Rain::Loop()
{
	hitbox.y += speed;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2, 50, colliders);


	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], &hitbox, &result) == SDL_TRUE)// he goin crash!
		{
			parent->DeleteRainDrop(this);
		}
	}

}

void Rain::Render()
{
	App->ren->Blit(spells, hitbox.x, hitbox.y, &blitrect, -2);
}
