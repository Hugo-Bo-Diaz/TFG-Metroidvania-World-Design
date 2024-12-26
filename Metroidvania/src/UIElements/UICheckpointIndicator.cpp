#include "UICheckpointIndicator.h"
#include "Application.h"
#include "Modules/Window.h"
#include "Modules/Render.h"
#include "Modules/Gui.h"
#include "EngineAPI.h"

UIcheckpointIndicator::UIcheckpointIndicator()
{
	
	int w = 1024;
	int h = 576;

	on_screen.x = 0;
	//on_screen.w = App->win->width;
	on_screen.w = w;
	on_screen.h = 64;
	//on_screen.y = App->win->height / 2 - on_screen.h / 2;
	on_screen.y = h / 2 - on_screen.h / 2;

	//float textsize = text->font_used->hsize*text->text.size()*text->scale;
	//float textheight = text->font_used->vsize;
	float textsize = 0;
	float textheight = 0;

	text_x = on_screen.w/2- textsize / 2;
	//text_y = App->win->height / 2 + text->font_used->vsize / 2;
	text_y = h / 2 + textheight/ 2;

	mText = "CHECKPOINT REACHED";
}

void UIcheckpointIndicator::Init()
{
	Engine->GetModule<::Render>().LoadTexture("Assets/UI/black_square_alpha.png", Texture);
	Engine->GetModule<::Render>().LoadFont("Assets/Fonts/Bebas-Regular.ttf", RXColor{ 255, 255, 255, 255}, 50, mFont);

	int lScreenW = 0;
	int lScreenH = 0;
	int lTextSizeX = 0;
	int lTextSizeY = 0;

	Engine->GetModule<::Render>().GetTextSize(mFont,mText.c_str(), lTextSizeX, lTextSizeY);
	Engine->GetModule<Window>().GetWindowSize(lScreenW, lScreenH);

	text_x = lScreenW / 2 - lTextSizeX / 2;
	text_y = lScreenH / 2 - lTextSizeY / 2;
}

void UIcheckpointIndicator::Loop()
{
	if (to_dissapear.Read() > max_time_to_dissapear)
	{
		Engine->GetModule<UserInterface>().RemoveElement(this);
	}

}

void UIcheckpointIndicator::Render()
{
	Engine->GetModule<::Render>().RenderText(mText.c_str(), mFont, text_x, text_y, -1000, {0,0,0,0}, RenderQueue::RENDER_UI);
	Engine->GetModule<::Render>().RenderTexture(Texture, on_screen.x, on_screen.y, {0,0,1024,64}, -40, RenderQueue::RENDER_UI);
}

UIcheckpointIndicator::~UIcheckpointIndicator()
{
}
