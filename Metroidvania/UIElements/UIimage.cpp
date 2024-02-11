#include "UIimage.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"

UIimage::UIimage(float _x, float _y, float _w, float _h, float _depth, const char * texture)
{
	x = _x;
	y = _y;
	global_x = x;
	global_y = y;
	screenspace.x = x;
	screenspace.y = y;
	screenspace.w = _w;
	screenspace.h = _h;

	depth = _depth;

	texture_name = texture;
}

void UIimage::Loop()
{
	UIelement::Loop();
}

void UIimage::Render()
{
	App->ren->BlitUI(App->tex->Get_Texture(texture_name.c_str()), global_x, global_y, &screenspace, depth);

	UIelement::Render();
}
