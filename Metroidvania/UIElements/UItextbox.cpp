#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "UItextbox.h"
#include "Physics.h"
#include "Input.h"
#include "Text.h"

UItextbox::UItextbox(const char * _author,const char* first_text, TextBoxColor color, int tiles_x, int tiles_y, int x, int y, float size, float speed)
{
	author = _author;

	plus_rect_y = color*32*3;//32 pixels for tile, 3 tiles for each color

	up_left.y += plus_rect_y;
	left.y += plus_rect_y;
	down_left.y += plus_rect_y;
	down.y += plus_rect_y;
	down_right.y += plus_rect_y;
	right.y += plus_rect_y;
	up_right.y += plus_rect_y;
	up.y += plus_rect_y;
	center.y += plus_rect_y;


	tile_width = tiles_x;
	tile_height = tiles_y;

	if (tile_width < 3)
		tile_width = 3;

	if (tile_height < 3)
		tile_height = 3;

	position_x = x;
	position_y = y;

	text_speed = speed;
	text_size = size;

	SDL_Color Black = { 1,1,1,1 };

	//mFont = App->txt->LoadFont("Assets/Fonts/font1.xml", SDL_Color{0,0,0,255},25);
	mFont = App->txt->LoadFont("Assets/Fonts/Bebas-Regular.ttf", SDL_Color{0,0,0,255},25);

	AddPanelToTextBox(first_text);

	TexTextBox = App->tex->Load_Texture("Assets/UI/textboxes.png");
	App->phy->PauseObjects();
}

void UItextbox::AddPanelToTextBox(const char * text)
{
	texts.push_back(text);
}

UItextbox::~UItextbox()
{
	App->phy->UnPauseObjects();
}

void UItextbox::Loop()
{
	current_letter += text_speed;

	if (current_letter > texts[current_text].size())
		current_letter = texts[current_text].size();

	if (App->inp->GetInput(BUTTON_1) == BUTTON_DOWN)
	{
		if (current_text == texts.size()-1 && current_letter >= texts[current_text].size())
		{
			App->gui->RemoveElement(this);
		}
		else if (current_letter < texts[current_text].size())
		{
			current_letter = texts[current_text].size();
		}
		else
		{
			++current_text;
			current_letter = 0;
		}

	}
}

void UItextbox::Render()
{
	//lets draw the 2 squares
	//BEEG SQUARE
	for (int i = 0; i<tile_width; ++i)
	{
		for (int j = 0; j < tile_height; ++j)
		{
			SDL_Rect* r = nullptr;
			if (i == 0 && j == 0)
			{
				r = &up_left;
			}
			else if (i == 0 && j == tile_height - 1)
			{
				r = &down_left;
			}
			else if (j == tile_height - 1 && i == tile_width - 1)
			{
				r = &down_right;
			}
			else if (i == tile_width - 1 && j == 0)
			{
				r = &up_right;
			}
			else if (i == 0)
			{
				r = &left;
			}
			else if (j == tile_height - 1)
			{
				r = &down;
			}
			else if (i == tile_width - 1)
			{
				r = &right;
			}
			else if (j == 0)
			{
				r = &up;
			}
			else
			{
				r = &center;
			}

			int real_x = position_x + i*32;
			int real_y = position_y + j*32;

			App->ren->Blit(TexTextBox,real_x,real_y,r,10, RenderQueue::RENDER_UI);
		}
		
	}
	//BLIT TEXT 12 12
	App->ren->BlitText(texts[current_text].substr(0, current_letter).c_str(), mFont, position_x + 12, position_y + 12, -10001, SDL_Color{0,0,0,255}, RenderQueue::RENDER_UI);
	//App->ren->BlitText(texts[current_text].substr(0, current_letter).c_str(), mFont, position_x + 12, position_y + 12 + text_size * App->txt->GetFont(mFont)->size, -10001, SDL_Color{0,0,0,255});

	if (author != "")
	{
		//SMALL SQUARE
		float author_size = 1 + (author.size() * App->txt->GetFont(mFont)->size) / 32;

		for (int i = 0; i < author_size; ++i)
		{
			SDL_Rect* r = nullptr;
			if (i == 0)
			{
				r = &small_left;
			}
			else if (i == author_size - 1)
			{
				r = &small_right;
			}
			else
			{
				r = &small_center;
			}

			int real_x = position_x + i * 32;
			int real_y = position_y - 40;

			App->ren->Blit(TexTextBox, real_x, real_y, r, 9, RenderQueue::RENDER_UI);
		}

		App->ren->BlitText(author.c_str(), mFont, position_x + 12, position_y - 8, -10001, SDL_Color{0,0,0,255}, RenderQueue::RENDER_UI);
	}

}
