#include "LavaSpell.h"
#include "Application.h"
#include "Render.h"
#include "Fire.h"
#include "Physics.h"

void LavaSpell::Loop()
{
	hitbox.y += speed;

	if (life.Read()>lifespan)// he goin crash!
	{
		parent->DeleteLava(this);
	}


}

void LavaSpell::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spells"), hitbox.x, hitbox.y, &blitrect, -2);
	//App->ren->DrawRect(&hitbox, 255, 255, 255, 255, true);
}
