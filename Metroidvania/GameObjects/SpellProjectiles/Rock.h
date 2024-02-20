#ifndef ROCK__H
#define ROCK__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class Rock : public GameObject
{
public:

	Rock();
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

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new Rock(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H
