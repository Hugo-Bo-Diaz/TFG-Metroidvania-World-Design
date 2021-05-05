#include "LavaSpell.h"
#include "Application.h"
#include "Render.h"
#include "Fire.h"
#include "Physics.h"

#include "CoalJumper.h"
#include "GroundedElemental.h"
#include "FlyingElemental.h"


void LavaSpell::Loop()
{
	hitbox.y += speed;

	if (life.Read()>lifespan)// he goin crash!
	{
		parent->DeleteLava(this);
	}

	std::vector<collision*> collisions;
	App->phy->GetCollisions(&hitbox, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{

		int direction = 0;
		if(hitbox.x<(*it)->object->collider->x)
		{
			direction = 1;
		}
		else
		{
			direction = -1;
		}

		if ((*it)->type == COAL_JUMPER)
		{
			((CoalJumper*)(*it)->object)->RecieveDamage(damage, direction);
		}
		if ((*it)->type == GROUNDED_ELEMENTAL)
		{
			((GroundedElemental*)(*it)->object)->RecieveDamage(damage, direction);
		}
		if ((*it)->type == FLYING_ELEMENTAL)
		{
			((FlyingElemental*)(*it)->object)->RecieveDamage(damage, direction);
		}
	}
	
}

void LavaSpell::Render()
{
	App->ren->Blit(App->tex->Get_Texture("spells"), hitbox.x, hitbox.y, &blitrect, -2);
	//App->ren->DrawRect(&hitbox, 255, 255, 255, 255, true);
}
