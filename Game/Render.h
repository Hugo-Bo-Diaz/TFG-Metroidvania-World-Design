#ifndef RENDER__H
#define RENDER__H

#include"PartsDef.h"
#include "SDL/include/SDL.h"
#include <queue>

struct TextPrint;

struct BlitItem
{
	int x;
	int y;
	SDL_Texture* tex;
	SDL_Rect* on_img;
	int depth;
	float parallax_factor_x;
	float parallax_factor_y;

	//rotation point and angle
	//SDL_Point img_center;
	float angle;

	bool operator<(const BlitItem& rhs) const
	{
		if (depth < rhs.depth)
			return true;
		else
			return false;
	}
};

struct BlitTile
{
	int x_tile;
	int y_tile;
	SDL_Texture* tex;
	int depth;
	float parallax_factor_x;
	float parallax_factor_y;

	float on_tile_x;
	float on_tile_y;
	float on_tile_w;
	float on_tile_h;

	bool operator<(const BlitItem& rhs) const
	{
		if (depth < rhs.depth)
			return true;
		else
			return false;
	}
};

struct BlitBackground
{
	SDL_Texture*tex;
 	bool repeat_y;
	float parallax_x;
	float parallax_y;
	int depth;

	bool operator<(const BlitItem& rhs) const
	{
		if (depth < rhs.depth)
			return true;
		else
			return false;
	}
};
struct BlitRect
{
	uint r;
	uint g;
	uint b;
	uint a;

	SDL_Rect* area;

	bool filled;
};

struct Trail
{
	SDL_Point* points;
	int amount;

	uint r;
	uint g;
	uint b;
};

struct TextBlit
{
	TextPrint* to_print;
	int x;
	int y;
};

class Render : public Part
{
public:
	Render();
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void Blit(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle = 0, float parallax_factor_x = 1, float parallax_factor_y = 1);
	void BlitMapTile(SDL_Texture* tex, int x_tile, int y_tile,SDL_Rect on_img, int depth, float parallax_factor_x = 1, float parallax_factor_y = 1);
	void BlitMapBackground(SDL_Texture* tex, int depth, bool repeat_y, float parallax_factor_x = 1, float parallax_factor_y = 1);
	void BlitUI(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle = 0);
	void BlitText(TextPrint* text,int x, int y);
	void DrawRect(SDL_Rect* area, uint r, uint g, uint b, uint a, bool filled);
	void DrawTrail(SDL_Point* point_array, int amount, int r = 255, int g = 255, int b= 255);

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

private:

	int		width;
	int		height;
	int		scale;
	
	std::vector<BlitItem*> blit_queue;
	std::vector<BlitItem*> ui_blit_queue;
	std::queue<TextBlit*> text_queue;
	std::queue<BlitRect*> quad_queue;
	std::queue<Trail*> trail_queue;
	std::vector<BlitTile*> tile_queue;
	std::vector<BlitBackground*> background_queue;
	

public:

	SDL_Renderer*	renderer;
	SDL_Color		background;

};

#endif // !RENDER__H
