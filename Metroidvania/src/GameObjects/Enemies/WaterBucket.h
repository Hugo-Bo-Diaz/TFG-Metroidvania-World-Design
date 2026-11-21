#ifndef WATER__BUCKET__H
#define WATER__BUCKET__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"
#include "Enemy.h"
#include "RXRect.h"
class Player;

typedef int AudioID;

enum WaterBucketState {
	WaterBucket_PATROL,
	WaterBucket_DROP
};

class WaterBucket : public Enemy
{
public:
	WaterBucket();
	WaterBucket(std::list<ObjectProperty*>& aList) { new (this) WaterBucket; };
	~WaterBucket();

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

	float patrol_speed = 1;
	float idle_speed = 0;
	float speed_x = patrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 4;

	bool RecieveDamage(int dmg, int direction);

	float animation_interval_patrol = 200;
	Timer animation_timer;

	Timer water_drop_timer;
	float total_time = 2000;
	float drop_time = 1000;
	bool mHasDroppedWater = false;

	Animation left;
	Animation right;
	Animation dump_left;
	Animation dump_right;
	particle_preset metal;
	particle_preset water;
	RXRect r18metalfirst;
	RXRect r19metalsecond;
	RXRect r18waterfirst;
	RXRect r19watersecond;
	RXRect r20waterthird;

	bool knocked_up = false;

	WaterBucketState current_state = WaterBucket_PATROL;

	RXRect aggro;

	RexTextureID waterbucket;
	RexTextureID particles;
};

#endif