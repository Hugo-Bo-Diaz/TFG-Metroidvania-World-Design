#ifndef LEAF__H
#define LEAF__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"

class ParticleEmitter;

class Leaf : public GameObject
{
public:

	Leaf();
	Leaf(std::list<ObjectProperty*>& aList) { new (this) Leaf; };

	void Fire(bool left_dir, float angle);

	bool Loop(float dt);
	bool Render();

	float speed = 15;
	int direction;

	float angle;
	float ratio_x;
	float ratio_y;

	Animation leaf_left;
	Animation leaf_right;

	ParticleEmitter* p;
	particle_preset grass;
	SDL_Rect r10grass;
	SDL_Rect r11grass;

	TextureID spells;
	TextureID particles;
};

#endif // !PLAYER__REP__H
