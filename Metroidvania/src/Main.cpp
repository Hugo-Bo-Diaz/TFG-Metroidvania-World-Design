#include <stdlib.h>
#include <stdio.h>
#include "Modules/ObjectManager.h"
#include "Modules/SceneController.h"
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
#include "GameObjects/Enemies/CloudSummonerProjectile.h"

#include "GameObjects/Hazards/HazardLava.h"
#include "GameObjects/Hazards/HazardLavaWaterfall.h"
#include "GameObjects/Hazards/HazardSpikes.h"
#include "GameObjects/Hazards/HazardsRockBlock.h"
#include "GameObjects/Hazards/CloudTrampoline.h"
#include "GameObjects/Portal.h"

#include "GameObjects/CheckPoint.h"
#include "GameObjects/FirstDialogue.h"
#include "GameObjects/TextBoxObject.h"
#include "GameObjects/DemoEndObject.h"

#include "SceneProcessing.h"
#include "Utils/Logger.h"
#include <functional>

int main(int argc, char* args[])
{
	bool lResult = false;
	Application* App = new Application("config.xml", lResult);
	if (!lResult)
	{
		Logger::Console_log(LogLevel::LOG_WARN,"EXITING: with errors (init)");
		return 1;
	}

	MetroidVaniaSceneProcessor::GetInstance().App = App;
	MetroidVaniaSceneProcessor::GetInstance().EngineInitialization();
	
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Player>("Player"));

	//demo and lore
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<TextBoxObject>("TextBoxObject"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FirstDialogue>("FirstDialogue"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<EndDemoObject>("EndDemoObject"));

	//flow
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Portal>("Portal"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<SpawnPoint>("SpawnPoint"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CheckPoint>("Checkpoint"));

	//hazards
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CloudTrampoline>("HazardsCloudTrampoline"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<HazardLava>("HazardLava"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<HazardLavaWaterfall>("HazardLavaWaterfall"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<HazardSpikes>("HazardSpikes"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<HazardRockBlock>("HazardRockBlock"));
	
	//projectiles
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<WindSlash>("ProjectileWindSlash"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Thorns>("ProjectileThorns"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Shockwave>("ProjectileShockwave"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Rock>("ProjectileRock"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Leaf>("ProjectileLeaf"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<IceBlock>("ProjectileIceBlock"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<IceShard>("ProjectileIceShard"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FireBall>("ProjectileFireBall"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<Cloud>("ProjectileCloud"));

	//pickups
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<MaxHealthPickup>("HealthChargePickup"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<MaxManaPickup>("ManaChargePickup"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FireSpellPickup>("SpellUnlockFire"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<GroundSpellPickup>("SpellUnlockGround"));

	//Enemies
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<ArmorTrap>("EnemyArmorTrap"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<ClingCreature>("EnemyClingCreature"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CloudMelee>("EnemyCloudMelee"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CloudSummoner>("EnemyCloudSummoner"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CloudSummonerProjectile>("EnemyCloudSummonerProjectile"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<CoalJumper>("EnemyCoalJumper"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FlyingAxe>("EnemyFlyingAxe"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FlyingShield>("EnemyFlyingShield"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<FlyingElemental>("EnemyFlyingElemental"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<GroundedElemental>("EnemyGroundedElemental"));
	App->mAPI->GetModule<ObjectManager>().AddFactory(new Factory<ShieldMonster>("EnemyShieldMonster"));


	App->mAPI->GetModule<SceneController>().AssignGameLoopFunction(std::bind(&MetroidVaniaSceneProcessor::SceneProcessingInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->mAPI->GetModule<SceneController>().AssignLoadFunction(std::bind(&MetroidVaniaSceneProcessor::SceneCreationInGame,&MetroidVaniaSceneProcessor::GetInstance()));
	App->mAPI->GetModule<SceneController>().LoadMap("Assets/maps/map0_entrance.tmx");
	App->Run();
	return 1;
}