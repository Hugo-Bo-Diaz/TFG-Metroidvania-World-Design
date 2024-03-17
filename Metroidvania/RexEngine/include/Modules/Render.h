#ifndef RENDER__H
#define RENDER__H

#include"PartsDef.h"
#include <queue>
#include "Textures.h"
#include "Text.h"
#include "SceneController.h"

#include "RXRect.h"
#include "RXColor.h"
#include "RXPoint.h"
#include "Part.h"


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
	void Blit(TextureID aTexID, int x, int y,const RXRect& rect_on_image, int depth, RenderQueue aQueue = RenderQueue::RENDER_GAME, float angle = 0, float parallax_factor_x = 1, float parallax_factor_y = 1, int center_x = -1,int center_y = -1);

	void BlitText(const char* text, FontID font, int x, int y, int depth, const RXColor& aColor,RenderQueue aQueue,bool ignore_camera = false);
	void DrawRect(const RXRect& area, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled,RenderQueue aQueue, int depth, bool ignore_camera = false);
	void DrawTrail(RXPoint* point_array, int amount, RenderQueue aQueue,bool aIgnoreCamera,int depth, uint8_t r = 255, uint8_t g = 255, uint8_t b= 255);

	long GetDrawCallsLastFrame();
	void CountDrawCall();

	SDL_Renderer* GetSDL_Renderer();

	class RenderImpl;
};

#endif // !RENDER__H
