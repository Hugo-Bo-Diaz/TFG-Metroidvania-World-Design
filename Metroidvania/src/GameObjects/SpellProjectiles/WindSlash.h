#ifndef WIND__SLASH__H
#define WIND__SLASH__H

#include "Modules/ObjectManager.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Textures.h"

class ParticleEmitter;

class WindSlash : public GameObject
{
public:

	WindSlash();
	WindSlash(std::list<ObjectProperty*>& aList) { new (this) WindSlash; };

	void Fire(bool left_dir);

	void Init();
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
};

#endif // !PLAYER__REP__H