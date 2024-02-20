#ifndef FLYING__SHIELD__H
#define FLYING__SHIELD__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class FlyingShield : public Enemy
{
public:
	FlyingShield();
	FlyingShield(float initial_y);
	~FlyingShield();

	void Init();
	bool Loop(float dt);
	bool Render();

	float health = 1;
	
	void RecieveDamage(int dmg, int direction);
	
	float x;
	float y;

	float initial_y;
	SDL_Rect* nextpos;

	//graphics
	Animation flying_left;
	Animation flying_right;

	SDL_Rect r1exp;
	particle_preset explosion;

	//patrol
	float patrol_width = 48 * 5;
	float current_distance = 0;

	float speed_x = 3;

	TextureID particles;
	TextureID floating_shield;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new FlyingShield(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif