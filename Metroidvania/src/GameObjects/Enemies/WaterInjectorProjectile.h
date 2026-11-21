#ifndef WATER__INJECTOR__PROJECTILE__H
#define WATER__INJECTOR__PROJECTILE__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

class WaterInjectorProjectile : public GameObject
{
public:

	WaterInjectorProjectile();
	WaterInjectorProjectile(std::list<ObjectProperty*>& aList) { new (this) WaterInjectorProjectile; };
	~WaterInjectorProjectile() {};

	void Fire(float speed_x, float speed_y);

	float speed_y;
	float accel_y = 0.5;

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	Animation projectile;

	int damage = 2;

	particle_preset water;
	RXRect r18waterfirst;
	RXRect r19watersecond;
	RXRect r20waterthird;

	RexTextureID injector;
	RexTextureID particles;

};

#endif // !PLAYER__REP__H
