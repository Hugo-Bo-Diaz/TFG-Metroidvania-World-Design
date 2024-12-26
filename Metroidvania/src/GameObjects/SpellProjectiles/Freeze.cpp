#include "Freeze.h"
#include "Application.h"
#include "Modules/Input.h"
#include "Modules/Render.h"
#include "Modules/Camera.h"
#include "Modules/Audio.h"

#include "../Enemies/CoalJumper.h"
#include "../Enemies/GroundedElemental.h"
#include "../Enemies/FlyingElemental.h"
#include "../Enemies/ArmorTrap.h"
#include "../Enemies/ShieldMonster.h"
#include "../Enemies/ClingingCreature.h"
#include "../Enemies/FlyingAxe.h"
#include "../Enemies/FlyingShield.h"

#include "../Hazards/HazardsRockBlock.h"

Freeze::Freeze()
{
}

Freeze::~Freeze()
{

}

void Freeze::Destroy()
{
	if (mTarget != nullptr)
	{
		mTarget->active = true;
	}
	Engine->GetModule<SceneController>().DeleteWall(mWallID);
}

void Freeze::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/Sprites/spells.png", spells);
	mSFXGroundHit = Engine->GetModule<Audio>().LoadSFX("Assets/SFX/hit_floor.wav");

	freeze.AddFrame({ 224,160,32,32 });
	freeze.mTexture = spells;
	FreezeTimer.Reset();
	FreezeTimer.Start();
}

bool Freeze::Loop(float dt)
{
	if (FreezeTimer.Read() > mTotalTime)
	{
		Engine->GetModule<SceneController>().DeleteObject(this);
	}

	return true;
}

bool Freeze::Render()
{
	Engine->GetModule<::Render>().RenderAnimation(freeze, collider.x, collider.y, -1000,RenderQueue::RENDER_GAME,0.0f,scale_x,scale_y);

	return true;
}

void Freeze::SetTarget(GameObject* aObject)
{
	if (aObject == nullptr)
		return;

	mTarget = aObject;

	collider.x = mTarget->collider.x;
	collider.y = mTarget->collider.y;
	collider.w = mTarget->collider.w;
	collider.h = mTarget->collider.h;

	mTarget->active = false;

	if (mWallID != -1)
	{
		Engine->GetModule<SceneController>().DeleteWall(mWallID);
	}
	mWallID = Engine->GetModule<SceneController>().AddWall(collider);

	scale_x = (float)mTarget->collider.w / 32.0f;
	scale_y = (float)mTarget->collider.h / 32.0f;
}