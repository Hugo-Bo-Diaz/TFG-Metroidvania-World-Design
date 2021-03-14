#include "UIspelldisplay.h"
#include "Application.h"
#include "Render.h"

UIspelldisplay::UIspelldisplay()
{
	SDL_Rect fire = SDL_Rect{ 0,0,52,64 };
	SDL_Rect wind = SDL_Rect{ 54,0,52,64 };
	SDL_Rect water = SDL_Rect{ 108,0,52,64 };
	SDL_Rect grass = SDL_Rect{ 162,0,52,64 };
	SDL_Rect ground = SDL_Rect{ 216,0,52,64 };

	books.push_back(fire);
	books.push_back(wind);
	books.push_back(water);
	books.push_back(grass);
	books.push_back(ground);

	left_border = { 334,0,64,82 };
	right_border = { 398,0,64,82 };

	left_cover = { 206,0,64,82 };
	right_cover = {270,0,64,82};
	square = { 134,0,68,82 };
}

void UIspelldisplay::Loop()
{
	UIelement::Loop();

	current_spell = player->current_spell;
}

void UIspelldisplay::Render()
{
	UIelement::Render();

	//render borders;
	App->ren->BlitUI(App->tex->Get_Texture("spell_display_base"), x, y, &left_border, 50);
	App->ren->BlitUI(App->tex->Get_Texture("spell_display_base"), x+160, y, &right_border, 50);//(32+8+31+8)*2

	App->ren->BlitUI(App->tex->Get_Texture("spell_display_base"), x, y, &left_cover, 40);
	App->ren->BlitUI(App->tex->Get_Texture("spell_display_base"), x + 160, y, &right_cover, 40);//(32+8+31+8)*2

	App->ren->BlitUI(App->tex->Get_Texture("spell_display_base"), x+78, y, &square, 40);


	//render book;
	App->ren->BlitUI(App->tex->Get_Texture("spell_books"),x+84,y+8, &books[current_spell],45);//(32+8)*2


}
