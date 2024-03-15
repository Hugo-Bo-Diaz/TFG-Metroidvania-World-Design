#ifndef GROUNDED__ELEMENTAL__H
#define GROUNDED__ELEMENTAL__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "Enemy.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"

enum GroundedElementalColor
{
	RED_GROUNDELEMENTAL,
	BROWN_GROUNDEDELEMENTAL
};

class GroundedElemental : public Enemy
{
public:
	GroundedElemental();
	GroundedElemental(std::list<ObjectProperty*>& aProperties);
	~GroundedElemental() {};

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	float patrol_speed = 1;
	float speed_x = patrol_speed;
	float speed_y = -10;

	float acceleration_y = 1.0;
	float health = 2;

	void RecieveDamage(int dmg, int direction);

	float animation_frame_interval=100;
	Timer animation_timer;
	Animation walking_left;
	Animation walking_right;
	particle_preset fire_ge_death;
	particle_preset stone_death;
	RXRect r14firegedeath;
	RXRect r15firegedeath;
	RXRect r16sandfirst;
	RXRect r17sandsecond;

	void SetAnimations(GroundedElementalColor c);
	bool knocked_up = false;

	GroundedElementalColor c;

	TextureID particles;
	TextureID groundelemental;

	AudioID mSFXHit;
};

#endif