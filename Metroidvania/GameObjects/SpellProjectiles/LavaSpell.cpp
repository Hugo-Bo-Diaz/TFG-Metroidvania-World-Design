#include "LavaSpell.h"
#include "Application.h"
#include "Render.h"
#include "../../Spells/Fire.h"
#include "Physics.h"

#include "../Enemies/CoalJumper.h"
#include "../Enemies/GroundedElemental.h"
#include "../Enemies/FlyingElemental.h"
#include "../Enemies/ArmorTrap.h"
#include "../Enemies/ShieldMonster.h"
#include "../Enemies/ClingingCreature.h"
#include "../Enemies/FlyingAxe.h"
#include "../Enemies/FlyingShield.h"


LavaSpell::LavaSpell(TextureID tex)
{
	spells = tex;
}

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

		if ((*it)->object->IsSameTypeAs<Enemy>())
		{
			((Enemy*)(*it)->object)->RecieveDamage(damage, direction);
		}
	}
	App->phy->ClearCollisionArray(collisions);
}

void LavaSpell::Render()
{
	App->ren->Blit(spells, hitbox.x, hitbox.y, &blitrect, -2);
	//App->ren->DrawRect(&hitbox, 255, 255, 255, 255, true);
}
