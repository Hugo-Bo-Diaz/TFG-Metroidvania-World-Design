#ifndef FREEZE__H
#define FREEZE__H

#include "Modules/SceneController.h"
#include "EngineElements/Animation.h"
#include "Modules/Render.h"
#include "Modules/Audio.h"
#include "../Enemies/Enemy.h"
#include <map>

class Freeze : public GameObject
{
public:

	Freeze();
	Freeze(std::list<ObjectProperty*>& aList) { new (this) Freeze; };
	~Freeze();

	void Init();
	bool Loop(float dt);
	bool Render();
	void Destroy();

	GameObject* mTarget;
	void SetTarget(GameObject* aObject);
	Timer FreezeTimer;
	float mTotalTime = 3000;
	int mWallID = -1;

	Animation freeze;
	RexTextureID spells;

	float scale_x = 1.0f;
	float scale_y = 1.0f;

	AudioID mSFXGroundHit;

	static std::map<Freeze*,Enemy*> mEnemiesFrozen;
};

#endif // !PLAYER__REP__H
