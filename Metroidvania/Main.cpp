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

#include "GameObjects/CheckPoint.h"
#include "GameObjects/FirstDialogue.h"
#include "GameObjects/TextBoxObject.h"
#include "GameObjects/DemoEndObject.h"

#include "SceneProcessing.h"
#include "Logger.h"
#include <functional>

int main(int argc, char* args[])
{
	bool lResult = false;
	App = new Application("config.xml", lResult);
	if (!lResult)
	{
		Logger::Console_log(LogLevel::LOG_WARN,"EXITING: with errors (init)");
		return 1;
	}

	MetroidVaniaSceneProcessor::GetInstance().EngineInitialization();

	App->phy->AddFactory(new Factory<Player>("Player"));

	//demo and lore
	App->phy->AddFactory(new Factory<TextBoxObject>("TextBoxObject"));
	App->phy->AddFactory(new Factory<FirstDialogue>("FirstDialogue"));
	App->phy->AddFactory(new Factory<EndDemoObject>("EndDemoObject"));

	//flow
	App->phy->AddFactory(new Factory<Portal>("Portal"));
	App->phy->AddFactory(new Factory<SpawnPoint>("SpawnPoint"));
	App->phy->AddFactory(new Factory<CheckPoint>("Checkpoint"));

	//hazards
	App->phy->AddFactory(new Factory<CloudTrampoline>("HazardsCloudTrampoline"));
	App->phy->AddFactory(new Factory<HazardLava>("HazardLava"));
	App->phy->AddFactory(new Factory<HazardLavaWaterfall>("HazardLavaWaterfall"));
	App->phy->AddFactory(new Factory<HazardSpikes>("HazardSpikes"));
	App->phy->AddFactory(new Factory<HazardRockBlock>("HazardRockBlock"));
	
	//projectiles
	App->phy->AddFactory(new Factory<WindSlash>("ProjectileWindSlash"));
	App->phy->AddFactory(new Factory<Thorns>("ProjectileThorns"));
	App->phy->AddFactory(new Factory<Shockwave>("ProjectileShockwave"));
	App->phy->AddFactory(new Factory<Rock>("ProjectileRock"));
	App->phy->AddFactory(new Factory<Leaf>("ProjectileLeaf"));
	App->phy->AddFactory(new Factory<IceBlock>("ProjectileIceBlock"));
	App->phy->AddFactory(new Factory<IceShard>("ProjectileIceShard"));
	App->phy->AddFactory(new Factory<FireBall>("ProjectileFireBall"));
	App->phy->AddFactory(new Factory<Cloud>("ProjectileCloud"));

	//pickups
	App->phy->AddFactory(new Factory<MaxHealthPickup>("HealthChargePickup"));
	App->phy->AddFactory(new Factory<MaxManaPickup>("ManaChargePickup"));
	App->phy->AddFactory(new Factory<FireSpellPickup>("SpellUnlockFire"));
	App->phy->AddFactory(new Factory<GroundSpellPickup>("SpellUnlockGround"));

	//Enemies
	App->phy->AddFactory(new Factory<ArmorTrap>("EnemyArmorTrap"));
	App->phy->AddFactory(new Factory<ClingCreature>("EnemyClingCreature"));
	App->phy->AddFactory(new Factory<CloudMelee>("EnemyCloudMelee"));
	App->phy->AddFactory(new Factory<CloudSummoner>("EnemyCloudSummoner"));
	App->phy->AddFactory(new Factory<CloudSummonerProjectile>("EnemyCloudSummonerProjectile"));
	App->phy->AddFactory(new Factory<CoalJumper>("EnemyCoalJumper"));
	App->phy->AddFactory(new Factory<FlyingAxe>("EnemyFlyingAxe"));
	App->phy->AddFactory(new Factory<FlyingShield>("EnemyFlyingShield"));
	App->phy->AddFactory(new Factory<FlyingElemental>("EnemyFlyingElemental"));
	App->phy->AddFactory(new Factory<GroundedElemental>("EnemyGroundedElemental"));
	App->phy->AddFactory(new Factory<ShieldMonster>("EnemyShieldMonster"));


	App->scn->AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->scn->AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->Run();
	return 1;
}