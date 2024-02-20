#ifndef THORNS__H
#define THORNS__H

#include "Physics.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class ParticleEmitter;

enum ThornsState {
	SEED,
	THORNS_ON_FLOOR
};

class Thorns : public GameObject
{
public:

	Thorns();

	void Fire(bool left_dir, float lifespan);

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

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new Thorns(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H
