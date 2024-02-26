#ifndef RENDER__H
#define RENDER__H

#include"PartsDef.h"
#include "SDL/include/SDL.h"
#include <queue>
#include "Textures.h"
#include "Text.h"
#include "SceneController.h"


struct TextPrint;
class ParticleEmitter;

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

	SDL_Color color;

	//rotation point and angle
	//SDL_Point img_center;
	int center_x;
	int center_y;
	float angle;

	bool ignore_camera = false;

	virtual void Blit(Render* aRender) = 0;

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

class BlitItemText : public BlitItem
{
public:
	std::string mText;
	FontID font_used;

	void Blit(Render* aRender);
};

class BlitTexture : public BlitItem
{
public:
	TextureID tex;
	SDL_Rect on_image;
	float parallax_x;
	float parallax_y;

	void Blit(Render* lRender);
};

class BlitLayer : public BlitItem
{
public:

	layer* layer;

	void Blit(Render* lRender);

	SDL_Rect GetImageRectFromId(tileset* t, int id);

};


class BlitBackground : public BlitTexture
{
public:

 	bool repeat_y;
	void Blit(Render* lRender);
};

class BlitParticles : public BlitTexture
{
public:
	ParticleEmitter* lEmmitter;

	void Blit(Render* lRender);
};

class BlitRect : public BlitItem
{
public:

	int w;
	int h;

	bool filled;
	void Blit(Render* lRender);
};

class BlitTrail : public BlitItem
{
public:

	SDL_Point* points;
	int amount;
	void Blit(Render* lRender);
};

class Render : public Part
{
public:
	Render();
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	//all textured
	void Blit(TextureID aTexID, int x, int y, SDL_Rect* rect_on_image, int depth, RenderQueue aQueue = RenderQueue::RENDER_GAME, float angle = 0, float parallax_factor_x = 1, float parallax_factor_y = 1, int center_x = -1,int center_y = -1);
	void BlitMapLayer(layer* layer);
	void BlitMapBackground(TextureID aTexID, int depth, bool repeat_y, float parallax_factor_x = 1, float parallax_factor_y = 1);
	//void BlitUI(TextureID aTexID, int x, int y, SDL_Rect* rect_on_image, int depth, float angle = 0);
	
	/*void BlitMapTile(SDL_Texture* tex, int x_tile, int y_tile,SDL_Rect on_img, int depth, float parallax_factor_x = 1, float parallax_factor_y = 1);
	void BlitMapLayer(layer* layer);*/

	void BlitText(const char* text, FontID font, int x, int y, int depth,SDL_Color aColor,RenderQueue aQueue);
	void DrawRect(SDL_Rect* area, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled,RenderQueue aQueue);
	void DrawTrail(SDL_Point* point_array, int amount, RenderQueue aQueue, Uint8 r = 255, Uint8 g = 255, Uint8 b= 255);

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

private:

	int		width;
	int		height;
	int		scale;
	
	//std::vector<BlitItem*> blit_queue;
	//std::vector<BlitItem*> ui_blit_queue;
	//std::queue<TextBlit*> text_queue;
	//std::queue<BlitRect*> quad_queue;
	//std::queue<Trail*> trail_queue;
	////std::vector<BlitTile*> tile_queue;
	//std::vector<BlitBackground*> background_queue;
	//

	std::priority_queue<BlitItem*,std::vector<BlitItem*>,Comparer> allQueue;
	std::priority_queue<BlitItem*,std::vector<BlitItem*>,Comparer> uiQueue;
	std::priority_queue<BlitItem*,std::vector<BlitItem*>,Comparer> debugQueue;


	std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* GetQueue(RenderQueue aQueue);

public:

	SDL_Renderer*	renderer;
	SDL_Color		background;

};

#endif // !RENDER__H
