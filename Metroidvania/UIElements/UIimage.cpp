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

	Tex = App->tex->Load_Texture(texture);
}

void UIimage::Loop()
{
	UIelement::Loop();
}

void UIimage::Render()
{
	App->ren->Blit(Tex, global_x, global_y, &screenspace, depth, RenderQueue::RENDER_UI);

	UIelement::Render();
}
