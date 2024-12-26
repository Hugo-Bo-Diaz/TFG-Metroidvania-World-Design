#ifndef WIND__SLASH__H
#define WIND__SLASH__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Render.h"

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

	int health = 2;
	int damage = 1;

	Animation windslash;

	ParticleEmitter* p;
	particle_preset windslash_part;
	RXRect r5slash;

	RexTextureID spells;
	RexTextureID particles;
	
	std::list<GameObject*> mObjectsPassed;
};

#endif // !PLAYER__REP__H
