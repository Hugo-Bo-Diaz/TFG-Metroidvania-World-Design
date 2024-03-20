#ifndef RENDER__H
#define RENDER__H

#include"PartsDef.h"
#include "Textures.h"
#include "Text.h"
#include "SceneController.h"

#include "RXRect.h"
#include "RXColor.h"
#include "RXPoint.h"
#include "Part.h"
#include "EngineElements/Animation.h"

enum RenderQueue
{
	RENDER_GAME = 0,
	RENDER_UI = 1,
	RENDER_DEBUG = 2,

	RENDER_MAX = 3
};


class BlitItem
{
public:
	//position
	int x;
	int y;
	int depth;

	RXColor color;

	//rotation point and angle
	//SDL_Point img_center;
	int center_x;
	int center_y;
	float angle;

	bool ignore_camera = false;

	virtual void Blit(Render& aRender, Camera& camera, Window& aWindow) = 0;

	void SetPosition(int aX, int aY)
	{
		x = aX;
		y = aY;
	}

	void SetCenter(int aCenterX, int aCenterY)
	{
		center_x = aCenterX;
		center_y = aCenterY;
	}

	bool operator<(const BlitItem& rhs) const
	{
		return rhs.depth > depth;
	}
};

class Comparer
{
public:
	bool operator() (BlitItem* itemA, BlitItem* itemB)
	{
		return itemB->depth > itemA->depth;
	}
};

struct SDL_Renderer;

class DLL_EXPORT Render : public Part
{
public:
	Render(EngineAPI& aAPI);

	//all textured
	void RenderTexture(TextureID aTexID, int x, int y,const RXRect& rect_on_image, int aDepth, RenderQueue aQueue = RenderQueue::RENDER_GAME, float angle = 0, float parallax_factor_x = 1, float parallax_factor_y = 1, int center_x = -1,int center_y = -1);
	void RenderAnimation(Animation& aAnimation, int x, int y, int aDepth = 0, RenderQueue aQueue = RenderQueue::RENDER_GAME, float angle = 0, float parallax_factor_x = 1, float parallax_factor_y = 1, int center_x = -1, int center_y = -1);

	void RenderText(const char* text, FontID font, int x, int y, int depth, const RXColor& aColor,RenderQueue aQueue,bool ignore_camera = false);
	void RenderRect(const RXRect& area, const RXColor& aColor, bool filled,RenderQueue aQueue, int depth, bool ignore_camera = false);
	void RenderTrail(RXPoint* point_array, int amount, RenderQueue aQueue,bool aIgnoreCamera,int depth, uint8_t r = 255, uint8_t g = 255, uint8_t b= 255);

	long GetDrawCallsLastFrame();
	void CountDrawCall();

	SDL_Renderer* GetSDL_Renderer();

	class RenderImpl;
};

#endif // !RENDER__H
