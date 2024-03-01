#ifndef SHOCKWAVE__H
#define SHOCKWAVE__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"

class ParticleEmitter;

class Shockwave : public GameObject
{
public:

	Shockwave();
	Shockwave(std::list<ObjectProperty*>& aList) { new (this) Shockwave; };
	~Shockwave();

	void Fire(bool left_dir, float speed);

	void Init();
	bool Loop(float dt);
	bool Render();

	float x_speed = 0;
	SDL_Rect* nextpos = nullptr;

	Animation shockwave_left;
	Animation shockwave_right;

	SDL_Point floor_check;

	ParticleEmitter* p;

	int damage = 2;
	int direction;

	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	SDL_Rect r8ground;
	SDL_Rect r9ground;

	TextureID spells;
	TextureID particles;
};

#endif // !PLAYER__REP__H
