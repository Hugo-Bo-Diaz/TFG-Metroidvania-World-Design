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

	SDL_Rect thorn_first = {128,0,32,32};
	SDL_Rect thorn_second = {160,0,32,32};
	SDL_Rect seed = {192,0,32,32};
	SDL_Rect last_thorn_segment = { 128,0,8,32 };
	
	ParticleEmitter* p;
	particle_preset grass;
	SDL_Rect r10grass;
	SDL_Rect r11grass;

	TextureID spells;
	TextureID particles;
};

#endif // !PLAYER__REP__H
