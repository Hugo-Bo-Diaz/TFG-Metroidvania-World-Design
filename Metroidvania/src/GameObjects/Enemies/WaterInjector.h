#ifndef WATER__INJECTOR__H
#define WATER__INJECTOR__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"
#include "Enemy.h"
#include "RXRect.h"
class Player;

typedef int AudioID;

enum WaterInjectorState {
	WaterInjector_PATROL,
	WaterInjector_INJECT
};

class WaterInjector : public Enemy
{
public:
	WaterInjector();
	WaterInjector(std::list<ObjectProperty*>& aList) { new (this) WaterInjector; };
	~WaterInjector();

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

	float patrol_speed = 1;
	float speed_x = patrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 2;

	bool RecieveDamage(int dmg, int direction);

	Animation left;
	Animation right;
	Animation injection;

	Timer animation_timer;
	float animation_interval_patrol = 200;

	float injection_Interval = 500;
	Timer injection_timer;
	int injection_phase = 0;
	int max_injection_phase = 3;

	bool knocked_up = false;

	GameObject* target;

	WaterInjectorState current_state = WaterInjector_PATROL;

	RXRect aggro;

	RexTextureID particles;
	RXRect r18waterfirst;
	RXRect r19watersecond;
	RXRect r20waterthird;
	RXRect r21waterdrop;
	particle_preset water_death;
	particle_preset water_drip;
	RexTextureID injector;

	AudioID mSFXHit;
};

#endif