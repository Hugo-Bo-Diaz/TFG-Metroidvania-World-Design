#ifndef FLYING__SHIELD__H
#define FLYING__SHIELD__H

#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

class FlyingShield : public Enemy
{
public:
	FlyingShield();
	FlyingShield(std::list<ObjectProperty*>& aList) { new (this) FlyingShield; };
	FlyingShield(float initial_y);
	~FlyingShield() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	float health = 1;
	
	bool RecieveDamage(int dmg, int direction);
	
	float x;
	float y;

	float initial_y;
	RXRect* nextpos;

	//graphics
	Animation flying_left;
	Animation flying_right;

	RXRect r1exp;
	particle_preset explosion;

	//patrol
	float patrol_width = 48 * 5;
	float current_distance = 0;

	float speed_x = 3;

	RexTextureID particles;
	RexTextureID floating_shield;

	AudioID mSFXHit;
	AudioID mSFXPing;
};

#endif