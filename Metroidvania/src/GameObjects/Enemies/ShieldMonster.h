#ifndef SHIELD__MONSTER__H
#define SHIELD__MONSTER__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"

class Player;

enum ShieldMonsterState {
	ShieldMonster_PATROL,
	ShieldMonster_CHASE,
	ShieldMonster_WINDUP,
	ShieldMonster_ATTACK
};

class ShieldMonster : public Enemy
{
public:
	ShieldMonster();
	ShieldMonster(std::list<ObjectProperty*>& aList) { new (this) ShieldMonster; };
	~ShieldMonster();

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

	RXRect* nextpos = nullptr;

	//patrol
	float partrol_speed = 1;
	RXRect aggro;
	bool isplayernearby;
	float animation_interval_patrol = 200;
	float patrol_range = 100;
	int distance_traveled;


	//chase
	float chase_speed = 2;
	RXRect range;
	bool isplayeronrange;
	float animation_interval_chase = 100;

	//windup
	float max_windupangle = 90;
	float winduptime = 1000;
	Timer windupTimer;

	//attack
	float attacktime = 100;
	Timer attackTimer;
	float totalanimation = 500;

	float speed_x = partrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 10;

	void RecieveDamage(int dmg, int direction);

	RXRect shield;
	float max_shield_height = 140;

	RXRect HitBox;
	bool hashitground = false;

	Animation left;
	Animation right;
	RXRect arm_left;
	RXRect arm_right;
	float arm_angle = 0;
	int gdirection = 0;

	particle_preset shield_monster_hit;
	particle_preset shield_monster_death;
	particle_preset stone_death;
	particle_preset metal;
	RXRect r8ground;
	RXRect r9ground;
	RXRect r16sandfirst;
	RXRect r17sandsecond;
	RXRect r18metalfirst;
	RXRect r19metalsecond;
	
	TextureID shield_monster;
	TextureID shield_monster_arm;
	TextureID particles;

	Timer animation_timer;

	bool knocked_up = false;

	GameObject* target;

	ShieldMonsterState current_state = ShieldMonster_PATROL;

	float realx;

	AudioID mSFXHit;
	AudioID mSFXPing;
};

#endif