#include "Render.h"
#include "Application.h"
#include "Window.h"
#include "Textures.h"
#include "Camera.h"
#include "Particles.h"
#include "Text.h"
#include "SceneController.h"

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

	for (int i = 0; i < background_queue.size(); ++i)
	{
		BlitBackground* item = background_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{
			
			int back_w, back_h;

			SDL_QueryTexture(item->tex,NULL,NULL,&back_w,&back_h);

			uint scale = App->win->GetScale();

			//calculate camera tile

			int cam_tile_x;
			int cam_tile_y;
			int j = 0;
			bool exit = false;
			while (!exit)
			{
				if (App->cam->position_x * item->parallax_x >= (j-1)*back_w && App->cam->position_x* item->parallax_x < j*back_w)
				{
					exit = true;
					cam_tile_x = j;
				}
				j++;
			}
			j = 0;
			exit = false;
			while (!exit)
			{
				if (App->cam->position_y* item->parallax_y >= (j - 1)*back_h && App->cam->position_y* item->parallax_y < j*back_h)
				{
					exit = true;
					cam_tile_y = j;
				}
				j++;
			}

			int rel_x = App->cam->position_x - cam_tile_x * back_w;
			int rel_y = App->cam->position_y - cam_tile_y * back_h;

			float images_that_fit_in_cam_x = App->cam->width / back_w;
			float images_that_fit_in_cam_y = App->cam->height / back_h;

			for (int i = 0; i <= images_that_fit_in_cam_x; ++i)
			{
				for (int j = 0; j <= images_that_fit_in_cam_y; ++j)
				{
					SDL_Rect rect;//DUAL LOOP THIS

					int pos_x = ((cam_tile_x - 1 + i) * back_w);
					int pos_y = ((cam_tile_y - 1 + j) * back_h);

					rect.x = pos_x + App->cam->GetCameraXoffset() *item->parallax_x;
					rect.y = pos_y + App->cam->GetCameraYoffset() *item->parallax_y;
					
					if (!item->repeat_y)
					{
						rect.y = 0;// App->scn->room_h * 48 - App->cam->height;
					}

					rect.w = back_w;
					rect.h = back_h;

					rect.w *= scale;
					rect.h *= scale;

					if (SDL_RenderCopyEx(renderer, item->tex, NULL, &rect, 0, NULL, SDL_FLIP_NONE) != 0)
					{
						printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
						ret = false;
					}

					if (!item->repeat_y)
					{
						break;
					}
				}
			}
		}
		//else {
		//printf("Texture not found! \n");
		//}
		delete item;
	}
	background_queue.clear();

	App->scn->RenderTiles();
	/*
	for (int i = 0; i < tile_queue.size(); ++i)
	{
		BlitTile* item = tile_queue[i];/*
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			float real_x = item->x_tile *item->on_tile_w;
			float real_y = item->y_tile *item->on_tile_h;

			uint scale = App->win->GetScale();

			SDL_Rect rect;
			rect.x = real_x *scale + App->cam->GetCameraXoffset() * item->parallax_factor_x;
			rect.y = real_y * scale + App->cam->GetCameraYoffset() * item->parallax_factor_y;

			rect.w = item->on_tile_w;
			rect.h = item->on_tile_h;

			rect.w *= scale;
			rect.h *= scale;

			SDL_Rect on_img;
			on_img.x = item->on_tile_x;
			on_img.y = item->on_tile_y;
			on_img.w = item->on_tile_w;
			on_img.h = item->on_tile_h;

			if (SDL_RenderCopyEx(renderer, item->tex, &on_img, &rect, 0, NULL, SDL_FLIP_NONE) != 0)
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
	tile_queue.clear();
	*/

	for (int i = 0; i < blit_queue.size(); ++i)
	{
		BlitItem* item = blit_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			uint scale = App->win->GetScale();

			SDL_Rect rect;
			rect.x = item->x * scale + App->cam->GetCameraXoffset() * item->parallax_factor_x;
			rect.y = item->y * scale + App->cam->GetCameraYoffset() * item ->parallax_factor_y;

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


				rect.w = (*it)->particles[i]->target_on_screen.w*(*it)->particles[i]->current_scale;
				rect.h = (*it)->particles[i]->target_on_screen.h*(*it)->particles[i]->current_scale;

				rect.x -= rect.w / 2;
				rect.y -= rect.h / 2;

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

		int y_level = 0;
		int length_so_far=0;

		for (int i = 0; i < print->current_letter; i++)//-97
		{
			//CHECK FOR NEXT LINE
			if (length_so_far > print->max_width)
			{
				y_level += 1;
				length_so_far = 0;
			}


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
				int y_scr = item->y- fontsizey + y_level * print->scale*(print->font_used->vsize + 5);//10 = SPACING
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
				int y_scr = item->y - fontsizey + y_level * print->scale*(print->font_used->vsize + 5);
				SDL_Rect on_screen = { x_scr,y_scr,fontsizex,fontsizey };
				
				length_so_far += on_screen.w;

				if (SDL_RenderCopyEx(renderer, item->to_print->font_used->font_texture, &on_img, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
				{
					printf("Cannot blit to screen. SDL_RenderCopy error: %s\n", SDL_GetError());
					ret = false;
				}
			}
			else if (print->text[i] == 32)
			{
				length_so_far += print->font_used->hsize*print->scale;
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

void Render::Blit(SDL_Texture* tex, int x, int y, SDL_Rect* rect_on_image, int depth, float angle, float parallax_factor_x, float parallax_factor_y)
{
	BlitItem* it = new BlitItem();
	it->depth = depth;
	it->on_img = rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = tex;

	//it->img_center = {center_x,center_y};
	it->angle = angle;
	it->parallax_factor_x = parallax_factor_x;
	it->parallax_factor_y = parallax_factor_y;

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
/*
void Render::BlitMapTile(SDL_Texture * tex, int x_tile, int y_tile,SDL_Rect on_img, int depth, float parallax_factor_x, float parallax_factor_y)
{
	BlitTile* it = new BlitTile();
	it->depth = depth;
	it->x_tile = x_tile;
	it->y_tile = y_tile;
	it->tex = tex;

	it->on_tile_w = on_img.w;
	it->on_tile_h = on_img.h;
	it->on_tile_x = on_img.x;
	it->on_tile_y = on_img.y;

	//it->img_center = {center_x,center_y};
	it->parallax_factor_x = parallax_factor_x;
	it->parallax_factor_y = parallax_factor_y;

	tile_queue.push_back(it);

}
*/
void Render::BlitMapBackground(SDL_Texture * tex, int depth, bool repeat_y, float parallax_factor_x, float parallax_factor_y)
{
	BlitBackground* it = new BlitBackground();

	it->depth = depth;
	it->tex = tex;

	it->repeat_y = repeat_y;

	//it->img_center = {center_x,center_y};
	it->parallax_x = parallax_factor_x;
	it->parallax_y = parallax_factor_y;

	background_queue.push_back(it);

	//order the elements
	
	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		int i = 0;
		int size = background_queue.size() - 1;
		for (i = 0; i < size; ++i)
		{
			if (background_queue[i]->depth < background_queue[i + 1]->depth)
			{
				swapped = true;
				//swap em;

				//save him into the temp
				BlitBackground* temp = new BlitBackground();
				*temp = *background_queue[i];

				//swap em
				*background_queue[i] = *background_queue[i + 1];

				//normalize everything
				*background_queue[i + 1] = *temp;
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

	/*
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
	}*/


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