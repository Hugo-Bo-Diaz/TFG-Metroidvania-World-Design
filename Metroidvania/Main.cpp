#include <stdlib.h>
#include <stdio.h>
#include "Physics.h"
#include "SceneController.h"
#include "Application.h"

#include "GameObjects/Player.h"

#include "Spells/Fire.h"

#include "GameObjects/SpellProjectiles/FireBall.h"
#include "GameObjects/SpellProjectiles/WindSlash.h"

#include "GameObjects/SpellProjectiles/IceShard.h"
#include "GameObjects/SpellProjectiles/IceBlock.h"

#include "GameObjects/SpellProjectiles/Cloud.h"
#include "GameObjects/SpellProjectiles/Leaf.h"
#include "GameObjects/SpellProjectiles/Thorns.h"

#include "GameObjects/SpellProjectiles/Rock.h"
#include "GameObjects/SpellProjectiles/Shockwave.h"

#include "GameObjects/Pickups/FireSpellPickup.h"
#include "GameObjects/Pickups/GroundSpellPickup.h"

#include "GameObjects/Pickups/MaxHealthPickup.h"
#include "GameObjects/Pickups/MaxManaPickup.h"

#include "GameObjects/Enemies/GroundedElemental.h"
#include "GameObjects/Enemies/FlyingElemental.h"
#include "GameObjects/Enemies/CoalJumper.h"
#include "GameObjects/Enemies/ArmorTrap.h"
#include "GameObjects/Enemies/ShieldMonster.h"
#include "GameObjects/Enemies/ClingingCreature.h"
#include "GameObjects/Enemies/FlyingAxe.h"
#include "GameObjects/Enemies/FlyingShield.h"
#include "GameObjects/Enemies/CloudMelee.h"
#include "GameObjects/Enemies/CloudSummoner.h"
#include "GameObjects/CloudSummonerProjectile.h"

#include "GameObjects/Hazards/HazardLava.h"
#include "GameObjects/Hazards/HazardLavaWaterfall.h"
#include "GameObjects/Hazards/HazardSpikes.h"
#include "GameObjects/Hazards/HazardsRockBlock.h"
#include "GameObjects/CloudTrampoline.h"
#include "Portal.h"
#include "GameObjects/EntityIDs.h"

#include "GameObjects/CheckPoint.h"
#include "GameObjects/FirstDialogue.h"
#include "GameObjects/TextBoxObject.h"
#include "GameObjects/DemoEndObject.h"

#include "SceneProcessing.h"

GameObject* CallB(objectId obj, std::map<std::string, float>* properties);
objectId TypeCallB(const char*);

int main(int argc, char* args[])
{
	App = new Application();
	//subscribe to error

	if (!App->Init())
	{
		printf("EXITING: with errors (init)\n");
		return 1;
	}

	App->scn->AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->scn->AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->phy->CallBack = CallB;
	App->phy->typeCallBack = TypeCallB;
	App->Run();


	return 1;
}

objectId TypeCallB(const char* obj)
{
	GameObject* r = nullptr;
	if (std::strcmp(obj, "SpellUnlockFire") == 0)
		return FIRE_SPELL_PICKUP_ID;
	else if (std::strcmp(obj, "SpellUnlockGround") == 0)
		return GROUND_SPELL_PICKUP_ID;
	else if (std::strcmp(obj, "HealthChargePickup") == 0)
		return MAX_HEALTH_PICKUP_ID;
	else if (std::strcmp(obj, "ManaChargePickup") == 0)
		return MAX_MANA_PICKUP_ID;
	else if (std::strcmp(obj, "HazardLava") == 0)
		return HAZARDS_LAVA_ID;
	else if (std::strcmp(obj, "HazardLavaWaterfall") == 0)
		return HAZARDS_LAVA_WATERFALL_ID;
	else if (std::strcmp(obj, "HazardSpikes") == 0)
		return HAZARDS_SPIKES_ID;
	else if (std::strcmp(obj, "HazardRockBlock") == 0)
		return HAZARDS_ROCK_BLOCK_ID;
	else if (std::strcmp(obj, "HazardsCloudTrampoline") == 0)
		return HAZARDS_CLOUD_TRAMPOLINE_ID;
	else if (std::strcmp(obj, "EnemyGroundedElemental") == 0)
		return GROUNDED_ELEMENTAL_ID;
	else if (std::strcmp(obj, "EnemyFlyingElemental") == 0)
		return FLYING_ELEMENTAL_ID;
	else if (std::strcmp(obj, "EnemyCoalJumper") == 0)
		return COAL_JUMPER_ID;
	else if (std::strcmp(obj, "EnemyArmorTrap") == 0)
		return ARMOR_TRAP_ID;
	else if (std::strcmp(obj, "EnemyShieldMonster") == 0)
		return SHIELD_MONSTER_ID;
	else if (std::strcmp(obj, "EnemyClingCreature") == 0)
		return CLING_CREATURE_ID;
	else if (std::strcmp(obj, "EnemyFlyingAxe") == 0)
		return FLYING_AXE_ID;
	else if (std::strcmp(obj, "EnemyFlyingShield") == 0)
		return FLYING_SHIELD_ID;
	else if (std::strcmp(obj, "EnemyCloudMelee") == 0)
		return CLOUD_MELEE_ID;
	else if (std::strcmp(obj, "EnemyCloudSummoner") == 0)
		return CLOUD_SUMMONER_ID;
	else if (std::strcmp(obj, "TextBoxObject") == 0)
		return TEXTBOXOBJECT_ID;
	else if (std::strcmp(obj, "DemoEndObject") == 0)
		return ENDDEMOOBJECT_ID;
	else if (std::strcmp(obj, "Player") == 0)
		return PLAYER_ID;
	else if (std::strcmp(obj, "Portal") == 0)
		return PORTAL_ID;
	else if (std::strcmp(obj, "SpawnPoint") == 0)
		return SPAWNPOINT_ID;

	return -1;
}


GameObject* CallB(objectId aobj,std::map<std::string,float>* properties)
{
	int obj = aobj;

	GameObject* r = nullptr;
	if (obj == PLAYER_ID)
		r = new Player();
	else if (obj == FIRE_BALL_ID)
		r = new FireBall();
	else if (obj == WIND_SLASH_ID)
		r = new WindSlash();
	else if (obj == ICE_SHARD_ID)
		r = new IceShard();
	else if (obj == ICE_BLOCK_ID)
		r = new IceBlock();
	else if (obj == CLOUD_ID)
		r = new Cloud();
	else if (obj == LEAF_ID)
		r = new Leaf();
	else if (obj == THORNS_ID)
		r = new Thorns();
	else if (obj == ROCK_ID)
		r = new Rock();
	else if (obj == SHOCKWAVE_ID)
		r = new Shockwave();
	else if (obj == FIRE_SPELL_PICKUP_ID)
		r = new FireSpellPickup();
	else if (obj == GROUND_SPELL_PICKUP_ID)
		r = new GroundSpellPickup();
	else if (obj == MAX_HEALTH_PICKUP_ID)
		r = new MaxHealthPickup();
	else if (obj == MAX_MANA_PICKUP_ID)
		r = new MaxManaPickup();
	else if (obj == GROUNDED_ELEMENTAL_ID)
		r = new GroundedElemental();
	//	else if (obj == FLYING_ELEMENTAL_ID)
	//		r = new FlyingElemental(y);//////////////////////////////
	else if (obj == COAL_JUMPER_ID)
		r = new CoalJumper();
	else if (obj == ARMOR_TRAP_ID)
		r = new ArmorTrap();
	else if (obj == SHIELD_MONSTER_ID)
		r = new ShieldMonster();
	else if (obj == CLING_CREATURE_ID)
		r = new ClingCreature();
	else if (obj == FLYING_AXE_ID)
		r = new FlyingAxe();
	else if (obj == FLYING_SHIELD_ID)
		r = new FlyingShield();
	//	else if (obj == CLOUD_MELEE_ID)
	//		r = new CloudMelee(x, y);//////////////////////////////////
	//	else if (obj == CLOUD_SUMMONER_ID)
	//		r = new CloudSummoner(x, y);/////////////////////////////////
	else if (obj == CLOUD_SUMMONER_PROJECTILE_ID)
		r = new CloudSummonerProjectile();
	else if (obj == HAZARDS_LAVA_ID)
		r = new HazardLava();
	else if (obj == HAZARDS_LAVA_WATERFALL_ID)
		r = new HazardLavaWaterfall();
	else if (obj == HAZARDS_SPIKES_ID)
		r = new HazardSpikes();
	else if (obj == HAZARDS_ROCK_BLOCK_ID)
		r = new HazardRockBlock();
	else if (obj == HAZARDS_CLOUD_TRAMPOLINE_ID)
		r = new CloudTrampoline();
	else if (obj == CHECKPOINT_ID)
		r = new CheckPoint();
	else if (obj == FIRST_DIALOGUE_ID)
		r = new FirstDialogue();
	else if (obj == TEXTBOXOBJECT_ID)
		r = new TextBoxObject();
	else if (obj == ENDDEMOOBJECT_ID)
		r = new EndDemoObject();
	else if (obj == PORTAL_ID)
	{
		r = new Portal();

		auto lID = properties->find("id_room");
		((Portal*)r)->id_destination_room = (*lID).second;
		lID = properties->find("id_point");
		((Portal*)r)->id_destination_point= (*lID).second;
		lID = properties->find("horizontal");
		if ((*lID).second == 0)
		{
			((Portal*)r)->horizontal = false;
		}
		else
		{
			((Portal*)r)->horizontal = true;
		}
	}
	else if (obj == SPAWNPOINT_ID)
	{
		r = new SpawnPoint();

		auto lID = properties->find("id");
		((SpawnPoint*)r)->id = (*lID).second;
	}
	//else
	//	r = new GameObject();


	return r;
}





//void OnApplicationError(const char* aError)
//{
//
//}
