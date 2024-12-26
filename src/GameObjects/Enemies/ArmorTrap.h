#ifndef ARMOR__TRAP__H
#define ARMOR__TRAP__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"
#include "Modules/Audio.h"
#include "Enemy.h"

class Player;

typedef int AudioID;

enum ArmorTrapState {
	ArmorTrap_IDLE,
	ArmorTrap_PATROL,
	ArmorTrap_CHASE
};

class ArmorTrap : public Enemy
{
public:
	ArmorTrap();
	ArmorTrap(std::list<ObjectProperty*>& aList) { new (this) ArmorTrap; };
	~ArmorTrap();

	void Init();
	bool Loop(float dt);
	bool Render();
	void RenderDebug();
	void Destroy();

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

	AudioID mSFXHit;
};

#endif