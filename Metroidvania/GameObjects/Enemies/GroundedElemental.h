#ifndef GROUNDED__ELEMENTAL__H
#define GROUNDED__ELEMENTAL__H

#include "Physics.h"
#include "Animation.h"
#include "Enemy.h"
#include "ParticleEmitter.h"
#include "Textures.h"

enum GroundedElementalColor
{
	RED_GROUNDELEMENTAL,
	BROWN_GROUNDEDELEMENTAL
};

class GroundedElemental : public Enemy
{
public:
	GroundedElemental();
	~GroundedElemental();

	bool Loop(float dt);
	bool Render();

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
	SDL_Rect r14firegedeath;
	SDL_Rect r15firegedeath;
	SDL_Rect r16sandfirst;
	SDL_Rect r17sandsecond;

	void SetAnimations(GroundedElementalColor c);
	bool knocked_up = false;

	GroundedElementalColor c;

	TextureID particles;
	TextureID groundelemental;
};

#endif