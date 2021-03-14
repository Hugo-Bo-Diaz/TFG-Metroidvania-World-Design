#include "Render.h"
#include "Application.h"
#include "Window.h"
#include "Textures.h"
#include "Camera.h"
#include "Particles.h"
#include "Text.h"

#include <stdio.h>

Render::Render()
{
	name = "Render";
	renderer = NULL;

}

bool Render::LoadConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	pugi::xml_node& color_node = config_node.child("bkg_color");

	background.r = color_node.attribute("r").as_float(0);
	background.g = color_node.attribute("g").as_float(0);
	background.b = color_node.attribute("b").as_float(0);
	background.a = color_node.attribute("a").as_float(0);

	printf("Create SDL rendering context\n");
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config_node.child("vsync").attribute("on").as_bool(true))//if config vsync
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		printf("Using vsync\n");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);
	if(!renderer)
	{
		ret = false;
	}
	return ret;
}

bool Render::CreateConfig(pugi::xml_node& config_node)
{
	bool ret = true;

	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

	pugi::xml_node & color_node = config_node.append_child("bkg_color");
	color_node.append_attribute("r") = 0.0;
	color_node.append_attribute("g") = 0.0;
	color_node.append_attribute("b") = 0.0;
	color_node.append_attribute("a") = 0.0;

	printf("Create SDL rendering context\n");
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	//vsync will be on by default
	config_node.append_child("vsync").append_attribute("on") = true;
	flags |= SDL_RENDERER_PRESENTVSYNC;
	printf("Using vsync\n");

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);
	if (!renderer)
	{
		ret = false;
	}
	return ret;
}

bool Render::Init()
{
	bool ret = true;

	return ret;
}

bool Render::Loop(float dt)
{
	bool ret = true;
	SDL_RenderClear(renderer);

	for (int i = 0; i < blit_queue.size(); ++i)
	{
		BlitItem* item = blit_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			uint scale = App->win->GetScale();

			SDL_Rect rect;
			rect.x = item->x * scale + App->cam->GetCameraXoffset() * item->parallax_factor;
			rect.y = item->y * scale + App->cam->GetCameraYoffset() * item ->parallax_factor;

			if (item->on_img != NULL)
			{
				rect.w = item->on_img->w;
				rect.h = item->on_img->h;
			}
			else
			{
				SDL_QueryTexture(item->tex, NULL, NULL, &rect.w, &rect.h);
			}

			rect.w *= scale;
			rect.h *= scale;

			if (SDL_RenderCopyEx(renderer, item->tex, item->on_img, &rect, item->angle, NULL, SDL_FLIP_NONE) != 0)
			{
				printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
				ret = false;
			}
		}
		//else {
			//printf("Texture not found! \n");
		//}
		delete item;
	}
	blit_queue.clear();

	std::list<ParticleEmitter*> emitters = *App->par->GetParticleList();

	for (std::list<ParticleEmitter*>::iterator it = emitters.begin(); it != emitters.end(); it++)
	{
		particle**first = (*it)->particles;

		for (int i = 0; i < MAX_PARTICLES; ++i)
		{
			if ((*it)->particles[i] != nullptr)
			{
				uint scale = App->win->GetScale();

				SDL_Rect rect;
				rect.x = (*it)->particles[i]->target_on_screen.x * scale +App->cam->GetCameraXoffset();
				rect.y = (*it)->particles[i]->target_on_screen.y * scale +App->cam->GetCameraYoffset();


				rect.w = (*it)->particles[i]->target_on_screen.w;
				rect.h = (*it)->particles[i]->target_on_screen.h;

				/*rect.w *= scale;
				rect.h *= scale;
				*/
				if (SDL_RenderCopyEx(renderer, (*it)->particles[i]->texture, (*it)->particles[i]->area_in_texture, &rect, (*it)->particles[i]->angle, NULL, SDL_FLIP_NONE) != 0)
				{
					printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
					ret = false;
				}
			}
		}
	}

	for (int i = 0; i < ui_blit_queue.size(); ++i)
	{
		BlitItem* item = ui_blit_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			uint scale = App->win->GetScale();

			SDL_Rect rect;
			rect.x = item->x * scale;
			rect.y = item->y * scale;

			if (item->on_img != NULL)
			{
				rect.w = item->on_img->w;
				rect.h = item->on_img->h;
			}
			else
			{
				SDL_QueryTexture(item->tex, NULL, NULL, &rect.w, &rect.h);
			}

			rect.w *= scale;
			rect.h *= scale;

			if (SDL_RenderCopyEx(renderer, item->tex, item->on_img, &rect, item->angle, NULL, SDL_FLIP_NONE) != 0)
			{
				printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
				ret = false;
			}
		}
		//else {
		//printf("Texture not found! \n");
		//}
		delete item;
	}
	ui_blit_queue.clear();

	while (!text_queue.empty())
	{
		TextBlit* item = text_queue.front();
		uint scale = App->win->GetScale();
		
		TextPrint* print = item->to_print;


		int length_so_far=0;
		for (int i = 0; i < print->current_letter; i++)//-97
		{
			//characters A to Z (65 - 90)
			if ((print->text[i] >= 65 && print->text[i] <= 90))
			{
				int fontsizex = print->font_used->hsize;
				int fontsizey = print->font_used->vsize;
				int x_on = ((print->text[i] - 65) % print->font_used->char_per_row) * fontsizex;
				int y_on = ((print->text[i] - 65) / print->font_used->char_per_row) * fontsizey;
				SDL_Rect on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale*1.3;
				fontsizex *= print->scale*1.3;

				int x_scr = item->x + length_so_far;
				int y_scr = item->y- fontsizey;
				SDL_Rect on_screen = { x_scr,y_scr,fontsizex,fontsizey};

				length_so_far += on_screen.w;

				if (SDL_RenderCopyEx(renderer, item->to_print->font_used->font_texture, &on_img, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
				{
					printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
					ret = false;
				}
			}
			//characters a to z (97 - 122)
			else if (print->text[i] >= 97 && print->text[i] <= 122)
			{
				int fontsizex = print->font_used->hsize;
				int fontsizey = print->font_used->vsize;
				int x_on = ((print->text[i] - 97) % print->font_used->char_per_row) *fontsizex;
				int y_on = ((print->text[i] - 97) / print->font_used->char_per_row) * fontsizey;
				SDL_Rect on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale;
				fontsizex *= print->scale;

				int x_scr = item->x + length_so_far;
				int y_scr = item->y - fontsizey;
				SDL_Rect on_screen = { x_scr,y_scr,fontsizex,fontsizey };
				
				length_so_far += on_screen.w;

				if (SDL_RenderCopyEx(renderer, item->to_print->font_used->font_texture, &on_img, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
				{
					printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
					ret = false;
				}
			}
			
		}
		delete item;
		text_queue.pop();
	}

	while (!quad_queue.empty())
	{
		BlitRect* item = quad_queue.front();
		uint scale = App->win->GetScale();

		SDL_Rect temp = { item->area->x + App->cam->GetCameraXoffset(),item->area->y + App->cam->GetCameraYoffset() ,item->area->w,item->area->h };

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, item->r, item->g, item->b, item->a);
		int result = (item->filled) ? SDL_RenderFillRect(renderer, &temp) : SDL_RenderDrawRect(renderer,&temp);

		if (result != 0)
		{
			printf("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
			ret = false;
		}

		delete item;
		quad_queue.pop();
	}

	while (!trail_queue.empty())
	{
		Trail* item = trail_queue.front();
		uint scale = App->win->GetScale();

		for (int i = 0; i < item->amount; ++i)
		{
			item->points[i].x += App->cam->GetCameraXoffset();
			item->points[i].y += App->cam->GetCameraYoffset();
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, item->r, item->g, item->b, 255);// it's a debug feature so it'll have max visibility
		int result = SDL_RenderDrawLines(renderer,item->points,item->amount);

		for (int i = 0; i < item->amount; ++i)
		{
			item->points[i].x -= App->cam->GetCameraXoffset();
			item->points[i].y -= App->cam->GetCameraYoffset();
		}


		if (result != 0)
		{
			printf("Cannot draw trail to screen. SDL_RenderFillRect error: %s", SDL_GetError());
			ret = false;
		}

		delete item;
		trail_queue.pop();
	}



	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return ret;
}

void Render::Blit(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle, float parallax_factor)
{
	BlitItem* it = new BlitItem();
	it->depth = depth;
	it->on_img = rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = tex;

	//it->img_center = {center_x,center_y};
	it->angle = angle;
	it->parallax_factor = parallax_factor;

	blit_queue.push_back(it);
	
	//order the elements
	
	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		int i=0;
		int size = blit_queue.size() - 1;
		for (i = 0; i < size ; ++i)
		{
			if (blit_queue[i]->depth < blit_queue[i+1]->depth)
			{
				swapped = true;
				//swap em;

				//save him into the temp
				BlitItem* temp = new BlitItem();
				*temp = *blit_queue[i];

				//swap em
				*blit_queue[i] = *blit_queue[i+1];

				//normalize everything
				*blit_queue[i + 1] = *temp;
				delete temp;
				
			}
		}
	}

}

void Render::BlitUI(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle)
{
	BlitItem* it = new BlitItem();
	it->depth = depth;
	it->on_img = rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = tex;

	//it->img_center = {center_x,center_y};
	it->angle = angle;

	ui_blit_queue.push_back(it);

	//order the elements

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		int i = 0;
		int size = ui_blit_queue.size() - 1;
		for (i = 0; i < size; ++i)
		{
			if (ui_blit_queue[i]->depth < ui_blit_queue[i + 1]->depth)
			{
				swapped = true;
				//swap em;

				//save him into the temp
				BlitItem* temp = new BlitItem();
				*temp = *ui_blit_queue[i];

				//swap em
				*ui_blit_queue[i] = *ui_blit_queue[i + 1];

				//normalize everything
				*ui_blit_queue[i + 1] = *temp;
				delete temp;

			}
		}
	}


}

void Render::BlitText(TextPrint * text, int x, int y)
{
	TextBlit* it = new TextBlit();
	it->x = x;
	it->y = y;
	it->to_print = text;

	text_queue.push(it);
}


void Render::DrawRect(SDL_Rect* area, uint r, uint g, uint b, uint a, bool filled)
{
	BlitRect* it = new BlitRect();
	it->area = area;
	it->r = r;
	it->g = g;
	it->b = b;
	it->a = a;
	it->filled = filled;

	quad_queue.push(it);
}

void Render::DrawTrail(SDL_Point* point_array, int amount, int r, int g, int b)
{
	Trail* it = new Trail();
	it->points = point_array;
	it->amount = amount;
	it->r = r;
	it->g = g;
	it->b = b;

	trail_queue.push(it);
}

bool Render::CleanUp()
{
	bool ret = true;

	printf("Quitting Render\n");
	SDL_DestroyRenderer(renderer);

	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
	active = false;

	return ret;
}