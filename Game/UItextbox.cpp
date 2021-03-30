#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "UItextbox.h"
#include "Physics.h"
#include "Input.h"

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
	author_print = App->txt->CreateText(author.c_str(), Black, (tile_width - 2) * 32, "", text_size);
	AddPanelToTextBox(first_text);

	author_print->current_letter = author_print->text.length();


	App->phy->PauseObjects();
}

void UItextbox::AddPanelToTextBox(const char * text)
{
	std::string t = text;

	texts.push_back(t.c_str());

	SDL_Color Black = {1,1,1,1};
	TextPrint* text_print = App->txt->CreateText(text, Black, (tile_width - 2) * 32, "", text_size);

	text_print->current_letter = 0;

	text_prints.push_back(text_print);
}

UItextbox::~UItextbox()
{
	App->phy->UnPauseObjects();

	for (std::vector<TextPrint*>::iterator it = text_prints.begin(); it != text_prints.end(); it++)
	{
		App->txt->DeleteText(*it);
	}
	App->txt->DeleteText(author_print);
}

void UItextbox::Loop()
{
	current_letter += text_speed;

	if (current_letter > texts[current_text].size())
		current_letter = texts[current_text].size();

	text_prints[current_text]->current_letter = current_letter;


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
	SDL_Texture* texboxtexture = App->tex->Get_Texture("text_boxes");

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

			App->ren->BlitUI(texboxtexture,real_x,real_y,r,10);
		}
		
	}
	//BLIT TEXT 12 12
	App->ren->BlitText(text_prints[current_text], position_x + 12, position_y + 12 + text_size * text_prints[current_text]->font_used->vsize);

	if (author != "")
	{
		//SMALL SQUARE
		float author_size = 1 + (author.size() * author_print->font_used->hsize * author_print->scale) / 32;

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

			App->ren->BlitUI(texboxtexture, real_x, real_y, r, 9);


		}

		App->ren->BlitText(author_print, position_x + 12, position_y - 8);
	}

}
