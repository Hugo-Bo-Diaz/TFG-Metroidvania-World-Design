#include "UICheckpointIndicator.h"
#include "Application.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Gui.h"

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

	Texture = App->tex->Load_Texture("Assets/UI/black_square_alpha.png");
}


void UIcheckpointIndicator::Loop()
{
	if (to_dissapear.Read() > max_time_to_dissapear)
	{
		App->gui->RemoveElement(this);
	}

}

void UIcheckpointIndicator::Render()
{
	App->ren->BlitText("CHECKPOINT REACHED", mFont, text_x, text_y, -1000, {0,0,0,0}, RenderQueue::RENDER_UI);
	App->ren->Blit(Texture, on_screen.x,on_screen.y,NULL,-40, RenderQueue::RENDER_UI);
}

UIcheckpointIndicator::~UIcheckpointIndicator()
{
}
