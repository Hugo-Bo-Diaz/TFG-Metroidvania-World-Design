#ifndef RENDER_IMPL__H
#define RENDER_IMPL__H

#include "../include/Modules/Render.h"
#include "PartImpl.h"
#include "SDL/include/SDL.h"
#include "SceneControllerImpl.h"

class ParticleEmitter;
class Font;

class Render::RenderImpl : public Part::Part_Impl
{
public:
	Render::RenderImpl(Render* aRender) :mPartInst(aRender){};

	void SetRenderInst(Render* aRender)
	{
		mPartInst = aRender;
	}
	void BlitMapBackground(TextureID aTexID, int depth, bool repeat_y, float parallax_factor_x = 1, float parallax_factor_y = 1);
	void BlitMapLayer(layer* layer);
	void BlitParticleEmitter(ParticleEmitter* emitter, RenderQueue aRenderQueue);

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	bool LoadConfig(pugi::xml_node& config_node);
	bool CreateConfig(pugi::xml_node& config_node);

private:
	std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* GetQueue(RenderQueue aQueue);

	int		width;
	int		height;
	int		scale;
	int		mDrawCallsLastFrame;

	std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer> allQueue;
	std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer> uiQueue;
	std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer> debugQueue;

	SDL_Renderer*	renderer;
	SDL_Color		background;

	friend class Render;
	Render* mPartInst;
};


class BlitItemText : public BlitItem
{
public:
	std::string mText;
	Font* font_used;
	SDL_Texture* lFontTexture;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitTexture : public BlitItem
{
public:
	SDL_Texture* tex;
	SDL_Rect on_image;
	float parallax_x;
	float parallax_y;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitLayer : public BlitItem
{
public:

	layer* layer;
	SDL_Texture* tex;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);

	SDL_Rect GetImageRectFromId(tileset* t, int id);

};


class BlitBackground : public BlitTexture
{
public:

	bool repeat_y;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitParticles : public BlitTexture
{
public:
	ParticleEmitter* lEmmitter;
	SDL_Texture* tex;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitRect : public BlitItem
{
public:

	int w;
	int h;

	bool filled;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitTrail : public BlitItem
{
public:

	SDL_Point* points;
	int amount;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

#endif