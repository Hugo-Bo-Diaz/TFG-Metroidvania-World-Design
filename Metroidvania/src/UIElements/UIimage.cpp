#include "UIimage.h"
#include "Application.h"
#include "Modules/Render.h"
#include "Modules/Textures.h"
#include "EngineAPI.h"

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

	mPath = texture;
}

void UIimage::Init()
{

	Tex = Engine->GetModule<Textures>().Load_Texture(mPath.c_str());
}

void UIimage::Loop()
{
	UIelement::Loop();
}

void UIimage::Render()
{
	Engine->GetModule<::Render>().RenderTexture(Tex, global_x, global_y, screenspace, depth, RenderQueue::RENDER_UI);

	UIelement::Render();
}
