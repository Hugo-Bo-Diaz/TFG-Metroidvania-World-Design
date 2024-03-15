#ifndef CLOUD__SUMMONER__H
#define CLOUD__SUMMONER__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"

enum CloudSummonerState
{
	CS_PATROL,
	CS_AGGRO
};

class CloudSummoner : public Enemy
{
public:
	CloudSummoner();
	CloudSummoner(std::list<ObjectProperty*>&) { new (this) CloudSummoner; };
	CloudSummoner(float initial_x, float initial_y);
	~CloudSummoner() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	CloudSummonerState state = CS_PATROL;
	CloudSummonerState last_state = CS_PATROL;
	float health = 1;

	void RecieveDamage(int dmg, int direction);

	float x;
	float y;

	float initial_y;
	float initial_x;
	RXRect* nextpos = nullptr;

	//graphics
	Animation facing_left;
	Animation facing_right;
	Animation facing_front;
	Animation book;

	RXRect r1exp;
	particle_preset explosion;

	//patrol
	float patrol_speed = 2.5;

	RXRect aggro = { 0,0,600,600 };

	//follow
	float min_distance_to_player = 200;
	float max_distance_to_player = 400;
	float speed_follow = 2;
	float max_distance_from_origin = 1000;
	GameObject* following;
	bool home = true;

	Timer shooting_timer;
	float shooting_timer_charge = 900;
	float shooting_timer_total_cycle = 4000;

	float projectile_speed = 6;

	//other
	float speed_x = patrol_speed;
	float speed_y = -10;

	//BOOK MOVEMENT
	float base_book_offset_x;
	float base_book_offset_y;

	float oscilated_book_x;
	float oscilated_book_y;

	float oscilated_book_strength = 16;
	int book_angle;

	//MOVEMENT OSCILATION
	float oscilated_x;
	float oscilated_y;

	float oscilated_diff_x;
	float oscilated_diff_y;

	int perm = 0;

	TextureID cloud_summoner;
	TextureID particles;

	AudioID mSFXHit;
};

#endif