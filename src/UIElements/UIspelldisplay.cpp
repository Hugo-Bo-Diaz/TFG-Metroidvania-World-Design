#include "UIspelldisplay.h"
#include "Application.h"
#include "Modules/Render.h"
#include "EngineAPI.h"
#include "../GameObjects/Player.h"

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

void UIspelldisplay::Init()
{
	lTexture = Engine->GetModule<Textures>().Load_Texture("Assets/UI/spell_location.png");
	lBooksTex = Engine->GetModule<Textures>().Load_Texture("Assets/UI/books.png"); 
}

void UIspelldisplay::Loop()
{
	UIelement::Loop();
}

void UIspelldisplay::Render()
{
	UIelement::Render();
	if (current_spell != NONE_UNLOCKED && player != nullptr)
	{
		current_spell = player->current_spell;
		if (current_spell > 5)
			current_spell = 0;
		if (current_spell < 0)
			current_spell = 0;

		//render borders;
		Engine->GetModule<::Render>().Blit(lTexture, x, y, &left_border, 50, RenderQueue::RENDER_UI);
		Engine->GetModule<::Render>().Blit(lTexture, x + 160, y, &right_border, 50, RenderQueue::RENDER_UI);//(32+8+31+8)*2

		Engine->GetModule<::Render>().Blit(lTexture, x, y, &left_cover, 40, RenderQueue::RENDER_UI);
		Engine->GetModule<::Render>().Blit(lTexture, x + 160, y, &right_cover, 40, RenderQueue::RENDER_UI);//(32+8+31+8)*2

		Engine->GetModule<::Render>().Blit(lTexture, x + 78, y, &square, 40, RenderQueue::RENDER_UI);


		//render book;

		Engine->GetModule<::Render>().Blit(lBooksTex, x + 84, y + 8, &books[current_spell], 45, RenderQueue::RENDER_UI);//(32+8)*2
	}

}

void UIspelldisplay::RenderDebug()
{
	Engine->GetModule<::Render>().DrawRect({(int)x,(int)y,left_border.w,left_border.h }, 255, 255, 255, 255, false, RenderQueue::RENDER_DEBUG, -10, true);
	Engine->GetModule<::Render>().DrawRect({ (int)x + 160, (int)y, right_border.w,right_border.h }, 255, 255, 255, 255, false, RenderQueue::RENDER_DEBUG, -10, true);

	Engine->GetModule<::Render>().DrawRect({ (int)x, (int)y, left_cover.w,left_cover.h }, 255, 255, 255, 255, false, RenderQueue::RENDER_DEBUG, -10, true);
	Engine->GetModule<::Render>().DrawRect({ (int)x + 160, (int)y, right_cover.w,right_cover.h }, 255, 255, 255, 255, false, RenderQueue::RENDER_DEBUG, -10, true);

	Engine->GetModule<::Render>().DrawRect({ (int)x + 78, (int)y, square.x,square.y }, 255,255,255,255,false, RenderQueue::RENDER_DEBUG,-10,true);
}