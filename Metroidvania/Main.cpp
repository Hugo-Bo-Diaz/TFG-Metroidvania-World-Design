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
#include "Logger.h"
#include <functional>


GameObject* CallB(objectId obj, std::map<std::string, float>* properties);
objectId TypeCallB(const char*);

int main(int argc, char* args[])
{
	App = new Application();
	//subscribe to error

	if (!App->Init())
	{
		Logger::Console_log(LogLevel::LOG_WARN,"EXITING: with errors (init)");
		return 1;
	}

	App->phy->AddFactory("Player",GameObject::GetTypeInfo<Player>(), &Player::Factory);

	//demo and lore
	App->phy->AddFactory("TextBoxObject", GameObject::GetTypeInfo<TextBoxObject>(), &TextBoxObject::Factory);
	App->phy->AddFactory("FirstDialog", GameObject::GetTypeInfo<FirstDialogue>(), &FirstDialogue::Factory);
	App->phy->AddFactory("EndDemoObject", GameObject::GetTypeInfo<EndDemoObject>(), &EndDemoObject::Factory);

	//flow
	App->phy->AddFactory("Portal",GameObject::GetTypeInfo<Portal>(), &Portal::Factory);
	App->phy->AddFactory("SpawnPoint",GameObject::GetTypeInfo<SpawnPoint>(), &SpawnPoint::Factory);
	App->phy->AddFactory("Checkpoint",GameObject::GetTypeInfo<CheckPoint>(), &CheckPoint::Factory);

	//hazards
	App->phy->AddFactory("HazardsCloudTrampoline",GameObject::GetTypeInfo<CloudTrampoline>(), &CloudTrampoline::Factory);
	App->phy->AddFactory("HazardLava",GameObject::GetTypeInfo<HazardLava>(), &HazardLava::Factory);
	App->phy->AddFactory("HazardLavaWaterfall",GameObject::GetTypeInfo<HazardLavaWaterfall>(), &HazardLavaWaterfall::Factory);
	App->phy->AddFactory("HazardSpikes",GameObject::GetTypeInfo<HazardSpikes>(), &HazardSpikes::Factory);
	App->phy->AddFactory("HazardRockBlock",GameObject::GetTypeInfo<HazardRockBlock>(), &HazardRockBlock::Factory);

	//projectiles
	App->phy->AddFactory("ProjectileWindSlash",GameObject::GetTypeInfo<WindSlash>(), &WindSlash::Factory);
	App->phy->AddFactory("ProjectileThorns",GameObject::GetTypeInfo<Thorns>(), &Thorns::Factory);
	App->phy->AddFactory("ProjectileShockwave",GameObject::GetTypeInfo<Shockwave>(), &Shockwave::Factory);
	App->phy->AddFactory("ProjectileRock",GameObject::GetTypeInfo<Rock>(), &Rock::Factory);
	App->phy->AddFactory("ProjectileLeaf",GameObject::GetTypeInfo<Leaf>(), &Leaf::Factory);
	App->phy->AddFactory("ProjectileIceBlock",GameObject::GetTypeInfo<IceBlock>(), &IceBlock::Factory);
	App->phy->AddFactory("ProjectileIceShard",GameObject::GetTypeInfo<IceShard>(), &IceShard::Factory);
	App->phy->AddFactory("ProjectileFireBall",GameObject::GetTypeInfo<FireBall>(), &FireBall::Factory);
	App->phy->AddFactory("ProjectileCloud",GameObject::GetTypeInfo<Cloud>(), &Cloud::Factory);

	//pickups
	App->phy->AddFactory("HealthChargePickup",GameObject::GetTypeInfo<MaxHealthPickup>(), &MaxHealthPickup::Factory);
	App->phy->AddFactory("ManaChargePickup",GameObject::GetTypeInfo<MaxManaPickup>(), &MaxManaPickup::Factory);
	App->phy->AddFactory("SpellUnlockFire",GameObject::GetTypeInfo<FireSpellPickup>(), &FireSpellPickup::Factory);
	App->phy->AddFactory("SpellUnlockGround",GameObject::GetTypeInfo<GroundSpellPickup>(), &GroundSpellPickup::Factory);

	//Enemies
	App->phy->AddFactory("EnemyArmorTrap",GameObject::GetTypeInfo<ArmorTrap>(), &ArmorTrap::Factory);
	App->phy->AddFactory("EnemyClingCreature",GameObject::GetTypeInfo<ClingCreature>(), &ClingCreature::Factory);
	App->phy->AddFactory("EnemyCloudMelee",GameObject::GetTypeInfo<CloudMelee>(), &CloudMelee::Factory);
	App->phy->AddFactory("EnemyCloudSummoner",GameObject::GetTypeInfo<CloudSummoner>(), &CloudSummoner::Factory);
	App->phy->AddFactory("EnemyCloudSummonerProjectile",GameObject::GetTypeInfo<CloudSummonerProjectile>(), &CloudSummonerProjectile::Factory);
	App->phy->AddFactory("EnemyCoalJumper",GameObject::GetTypeInfo<CoalJumper>(), &CoalJumper::Factory);
	App->phy->AddFactory("EnemyFlyingAxe",GameObject::GetTypeInfo<FlyingAxe>(), &FlyingAxe::Factory);
	App->phy->AddFactory("EnemyFlyingShield",GameObject::GetTypeInfo<FlyingShield>(), &FlyingShield::Factory);
	App->phy->AddFactory("EnemyFlyingElemental",GameObject::GetTypeInfo<FlyingElemental>(), &FlyingElemental::Factory);
	App->phy->AddFactory("EnemyGroundedElemental",GameObject::GetTypeInfo<GroundedElemental>(), &GroundedElemental::Factory);
	App->phy->AddFactory("EnemyShieldMonster",GameObject::GetTypeInfo<ShieldMonster>(), &ShieldMonster::Factory);


	std::string str = std::type_index(typeid(ArmorTrap)).name();
	Logger::Console_log(LogLevel::LOG_DEBUG, std::type_index(typeid(Portal)).name());


	App->scn->AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->scn->AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->Run();


	return 1;
}

objectId TypeCallB(const char* obj)
{
	GameObject* r = nullptr;

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
