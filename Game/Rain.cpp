#include "Rain.h"
#include "Application.h"
#include "Render.h"
#include "Cloud.h"
#include "Physics.h"
void Rain::Loop()
{
	hitbox.y += speed;

	std::vector<SDL_Rect*> colliders;
	App->phy->GetNearbyWalls(hitbox.x + hitbox.w / 2, hitbox.y + hitbox.h / 2, 50, colliders);

	App->ren->Blit(App->tex->Get_Texture("rain"), hitbox.x, hitbox.y, &blitrect, -2);

	for (int i = 0; i < colliders.size(); ++i)
	{
		SDL_Rect result;
		if (SDL_IntersectRect(colliders[i], &hitbox, &result) == SDL_TRUE)// he goin crash!
		{
			parent->DeleteRainDrop(this);
		}
	}

}
