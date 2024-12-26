#ifndef DARK__CLOUD__H
#define DARK__CLOUD__H

#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Audio.h"

class DarkCloud : public Enemy
{
public:
	DarkCloud();
	DarkCloud(std::list<ObjectProperty*>& aProperties) {};
	~DarkCloud() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	float health = 5;
	
	bool RecieveDamage(int dmg, int direction);
	void ResetAngle(int dir);
	
	float lCurrentAngle = 0;

	float x;
	float y;
	RXRect* nextpos;

	//graphics
	Animation flying;

	RXRect r1exp;
	particle_preset explosion;

	//other
	float speed_x = 0;
	float speed_y = 0;

	RexTextureID flyingelemental;
	RexTextureID particles;

	AudioID mSFXHit;
};

#endif