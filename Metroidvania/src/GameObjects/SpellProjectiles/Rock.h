#ifndef ROCK__H
#define ROCK__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class Rock : public GameObject
{
public:

	Rock();
	Rock(std::list<ObjectProperty*>& aList) { new (this) Rock; };
	~Rock();
	void Fire(bool left_dir, float angle, float speed, float gravity);

	bool Loop(float dt);
	bool Render();

	float x_speed = 0;
	float y_speed = 0;
	float gravity = 0;

	int damage = 4;
	int direction;
	
	Animation rock_sprite;

	particle_preset groundcontact;
	particle_preset rockblockexplosion;
	SDL_Rect r8ground;
	SDL_Rect r9ground;

	TextureID spells;
	TextureID particles;
};

#endif // !PLAYER__REP__H
