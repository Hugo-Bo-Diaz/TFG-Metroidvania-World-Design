#ifndef RENDER__H
#define RENDER__H

#include"PartsDef.h"
#include "SDL/include/SDL.h"
#include <queue>

struct BlitItem
{
	int x;
	int y;
	SDL_Texture* tex;
	SDL_Rect* on_img;
	int depth;

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

class Render : public Part
{
public:
	Render();
	bool Init();
	bool Loop(float dt);
	bool CleanUp();

	void Blit(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle = 0);
	void DrawRect(SDL_Rect* area, uint r, uint g, uint b, uint a, bool filled);
	void DrawTrail(SDL_Point* point_array, int amount, int r = 255, int g = 255, int b= 255);

	bool LoadConfig(pugi::xml_node&);
	bool CreateConfig(pugi::xml_node&);

private:

	int		width;
	int		height;
	int		scale;
	
	std::vector<BlitItem*> blit_queue;
	std::queue<BlitRect*> quad_queue;
	std::queue<Trail*> trail_queue;

public:

	SDL_Renderer*	renderer;
	SDL_Color		background;

};

#endif // !RENDER__H
