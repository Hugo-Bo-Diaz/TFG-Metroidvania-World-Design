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
	BlitItemText(const char* aText, Font* aFontUsed, SDL_Texture* aTexture) 
		: mText(std::string(aText)), font_used(aFontUsed), lFontTexture(aTexture) {};

	std::string mText;
	Font* font_used;
	SDL_Texture* lFontTexture;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitTexture : public BlitItem
{
public:
	BlitTexture(SDL_Texture* aTex, SDL_Rect& aOnImage, float aParallax_x, float aParallax_y) 
		: tex(aTex), on_image(aOnImage), parallax_x(aParallax_x), parallax_y(aParallax_y) {};

	SDL_Texture* tex;
	SDL_Rect on_image;
	float parallax_x;
	float parallax_y;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitLayer : public BlitItem
{
public:
	BlitLayer(SDL_Texture* aTexture, layer* aLayer) : tex(aTexture), mLayer(aLayer) {};

	layer* mLayer;
	SDL_Texture* tex;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);

	SDL_Rect GetImageRectFromId(tileset* t, int id);

};


class BlitBackground : public BlitTexture
{
public:
	BlitBackground(SDL_Texture* aTexID, int aDepth, bool aRepeat_y, float aParallax_factor_x, float aParallax_factor_y) 
		: BlitTexture(aTexID, SDL_Rect{ 0,0,0,0 }, aParallax_factor_x, aParallax_factor_y), repeat_y(aRepeat_y) {
		depth = aDepth;
	};
	bool repeat_y;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitParticles : public BlitItem
{
public:
	BlitParticles(SDL_Texture* aTexture, ParticleEmitter* aEmmitter) : tex(aTexture), lEmmitter(aEmmitter) {};

	ParticleEmitter* lEmmitter;
	SDL_Texture* tex;

	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitRect : public BlitItem
{
public:
	BlitRect(const RXRect& aRect, bool aFilled) : w(aRect.w), h(aRect.h), filled(aFilled) {
		x = aRect.x;
		y = aRect.y;
	}

	int w;
	int h;

	bool filled;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

class BlitTrail : public BlitItem
{
public:
	BlitTrail(SDL_Point* aPoint, int aAmount, int aDepth) : points(aPoint), amount(aAmount) { depth = aDepth; }

	SDL_Point* points;
	int amount;
	void Blit(Render& aRender, Camera& camera, Window& aWindow);
};

#endif