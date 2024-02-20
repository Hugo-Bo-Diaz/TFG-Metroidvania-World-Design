#ifndef WIND__SLASH__H
#define WIND__SLASH__H

#include "Physics.h"
#include "Animation.h"
#include "ParticleEmitter.h"
#include "Textures.h"

class ParticleEmitter;

class WindSlash : public GameObject
{
public:

	WindSlash();

	void Fire(bool left_dir);

	bool Loop(float dt);
	bool Render();

	float speed = 20;
	int direction;

	Animation windslash;

	ParticleEmitter* p;
	particle_preset windslash_part;
	SDL_Rect r5slash;

	TextureID spells;
	TextureID particles;

	static GameObject* Factory(std::list<ObjectProperty*>&) { return new WindSlash(); };
	std::type_index GetTypeInfo() { return std::type_index(typeid(*this)); }
};

#endif // !PLAYER__REP__H
