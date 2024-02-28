#ifndef ARMOR__TRAP__H
#define ARMOR__TRAP__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"
#include "Textures.h"
#include "Enemy.h"

class Player;

enum ArmorTrapState {
	ArmorTrap_IDLE,
	ArmorTrap_PATROL,
	ArmorTrap_CHASE
};

class ArmorTrap : public Enemy
{
public:
	ArmorTrap();
	~ArmorTrap();

	bool Loop(float dt);
	bool Render();
	void RenderDebug();

	float patrol_speed = 1;
	float chase_speed = 3;
	float idle_speed = 0;
	float speed_x = patrol_speed;
	float speed_y = 0;

	float acceleration_y = 1.0;
	float health = 4;

	void RecieveDamage(int dmg, int direction);

	float animation_interval_chase = 100;
	float animation_interval_patrol = 200;

	Animation left;
	Animation right;
	SDL_Rect idle;
	particle_preset fire_ge_death;
	particle_preset metal;
	SDL_Rect r14firegedeath;
	SDL_Rect r15firegedeath;
	SDL_Rect r18metalfirst;
	SDL_Rect r19metalsecond;

	Timer animation_timer;

	bool knocked_up = false;

	GameObject* target;

	ArmorTrapState current_state = ArmorTrap_IDLE;

	SDL_Rect aggro;

	bool isplayernearby;

	TextureID armortrap;
	TextureID particles;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new ArmorTrap(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif