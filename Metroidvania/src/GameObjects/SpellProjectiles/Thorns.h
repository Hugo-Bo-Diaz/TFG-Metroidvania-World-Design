#ifndef THORNS__H
#define THORNS__H

#include "Modules/ObjectManager.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

enum ThornsState {
	SEED,
	THORNS_ON_FLOOR
};

class Thorns : public GameObject
{
public:

	Thorns();
	Thorns(std::list<ObjectProperty*>& aList) { new (this) Thorns; };

	void Fire(bool left_dir, float lifespan);

	void Init();
	bool Loop(float dt);
	bool Render();

	float speed = 5;
	float y_speed = -5;
	float gravity = 0.5;
	int direction;

	float expansion_speed = 8;
	bool expand = true;

	float lifespan = 5000;
	Timer life_timer;

	ThornsState state = SEED;

	Animation thorn_first;
	Animation thorn_second;
	Animation seed;
	Animation last_thorn_segment;
	
	ParticleEmitter* p;
	particle_preset grass;
	RXRect r10grass;
	RXRect r11grass;

	TextureID spells;
	TextureID particles;
};

#endif // !PLAYER__REP__H
