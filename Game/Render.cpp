#include "Render.h"
#include "Application.h"
#include "Window.h"
#include "Textures.h"
#include "Camera.h"
#include "Particles.h"
#include "Text.h"
#include "SceneController.h"
#include "Logger.h"

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

	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config_node.child("vsync").attribute("on").as_bool(true))//if config vsync
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		Logger::Console_log(LogLevel::LOG_INFO, "Using vsync");
	}

	Logger::Console_log(LogLevel::LOG_INFO, "Create SDL rendering context");
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

	pugi::xml_node & color_node = config_node.append_child("bkg_color");
	color_node.append_attribute("r") = 0.0;
	color_node.append_attribute("g") = 0.0;
	color_node.append_attribute("b") = 0.0;
	color_node.append_attribute("a") = 0.0;

	//vsync will be on by default
	config_node.append_child("vsync").append_attribute("on") = true;
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


	App->cam->screenarea.x = App->cam->position_x;
	App->cam->screenarea.y = App->cam->position_y;

	for (int i = 0; i < background_queue.size(); ++i)
	{
		BlitBackground* item = background_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{
			
			int back_w, back_h;

			SDL_QueryTexture(item->tex,NULL,NULL,&back_w,&back_h);

			float scale = App->win->GetScale();

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

					int pos_x = ((cam_tile_x - 1 + i) * back_w)*scale;
					int pos_y = ((cam_tile_y - 1 + j) * back_h)*scale;

					rect.x = pos_x + App->cam->GetCameraXoffset()*scale *item->parallax_x;
					rect.y = pos_y + App->cam->GetCameraYoffset()*scale *item->parallax_y;
					
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
						std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
						errstr += SDL_GetError();
						Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
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
		//Logger::Console_log(LogLevel::LOG_ERROR,"Texture not found!");
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
				printf("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
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

	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		int i = 0;
		int size = blit_queue.size() - 1;
		for (i = 0; i < size; ++i)
		{
			if (blit_queue[i]->depth < blit_queue[i + 1]->depth)
			{
				swapped = true;
				//swap em;

				//save him into the temp
				BlitItem* temp = new BlitItem();
				*temp = *blit_queue[i];

				//swap em
				*blit_queue[i] = *blit_queue[i + 1];

				//normalize everything
				*blit_queue[i + 1] = *temp;
				delete temp;

			}
		}
	}

	for (int i = 0; i < blit_queue.size(); ++i)
	{
		BlitItem* item = blit_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			float scale = App->win->GetScale();

			SDL_Rect rect;


			rect.x = (float)item->x * scale + App->cam->GetCameraXoffset() * item->parallax_factor_x;
			rect.y = (float)item->y * scale + App->cam->GetCameraYoffset() * item ->parallax_factor_y;

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

			SDL_Point* p = NULL;

			if (item->center_x != -1 && item->center_y != -1)
			{
				p = new SDL_Point();
				*p = { item->center_x,item->center_y };
			}
			
			

			if (SDL_RenderCopyEx(renderer, item->tex, item->on_img, &rect, item->angle, p, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
				//ret = false;
			}

			if (item->center_x != -1 && item->center_y != -1)
			{
				delete p;
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
				float scale = App->win->GetScale();

				SDL_Rect rect;
				rect.x = (*it)->particles[i]->target_on_screen.x * scale +App->cam->GetCameraXoffset();
				rect.y = (*it)->particles[i]->target_on_screen.y * scale +App->cam->GetCameraYoffset();


				rect.w = (*it)->particles[i]->target_on_screen.w*(*it)->particles[i]->current_scale;
				rect.h = (*it)->particles[i]->target_on_screen.h*(*it)->particles[i]->current_scale;

				rect.x -= rect.w / 2;
				rect.y -= rect.h / 2;

				rect.w *= scale;
				rect.h *= scale;
				
				if (SDL_RenderCopyEx(renderer, (*it)->particles[i]->texture, (*it)->particles[i]->area_in_texture, &rect, (*it)->particles[i]->angle, NULL, SDL_FLIP_NONE) != 0)
				{
					std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
					errstr += SDL_GetError();
					Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
					ret = false;
				}
			}
		}
	}
	swapped = true;
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
	for (int i = 0; i < ui_blit_queue.size(); ++i)
	{
		BlitItem* item = ui_blit_queue[i];
		//if (App->tex->Valid_Texture(item->tex))//validation method???
		{

			float scale = App->win->GetScale();

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
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
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
		float scale = App->win->GetScale();
		
		TextPrint* print = item->to_print;



		int y_level = 0;
		int length_so_far=0;

		int iterator = 0;

		for (int j = 0; j < print->words.size(); ++j)
		{
			//check line
			int fontsizex = print->font_used->hsize;
			int fontsizey = print->font_used->vsize;

			int length_of_current_word = print->words[j].size()*fontsizex*print->scale*scale;

			if ((length_of_current_word + length_so_far) > print->max_width*scale)
			{
				//printf("%d %s ", length_of_current_word + length_so_far - print->max_width, print->words[j].c_str());

				y_level += 1;
				length_so_far = 0;
			}
			else if (j != 0)
			{
				length_so_far += fontsizex*scale;
			}

			for (int i = 0;i< print->words[j].size(); ++i)
			{
				fontsizex = print->font_used->hsize;
				fontsizey = print->font_used->vsize;

				SDL_Rect on_img;
				
				//characters 0 to 9
				if (print->words[j][i] >= 48 && print->words[j][i] <= 57)
				{

					int x_on = print->font_used->hsize*(print->words[j][i] - 48);
					int y_on = print->font_used->hsize * 3;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;


				}

				//character ,
				if (print->words[j][i] == 44)
				{
					int x_on = 0;
					int y_on = print->font_used->hsize * 4;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;

				}

				//character !
				if (print->words[j][i] == 33)
				{
					int x_on = print->font_used->hsize * 1;
					int y_on = print->font_used->hsize * 4;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;
				}


				//character .
				if (print->words[j][i] == 46)
				{
					int x_on = print->font_used->hsize * 3;
					int y_on = print->font_used->hsize * 4;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;
				}

				//character '
				if (print->words[j][i] == 39)
				{
					int x_on = print->font_used->hsize * 4;
					int y_on = print->font_used->hsize * 4;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;
				}

				//character /
				if (print->words[j][i] == 47)
				{
					int x_on = print->font_used->hsize * 2;
					int y_on = print->font_used->hsize * 4;

					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;

				}


				//characters A to Z (65 - 90)
				if ((print->words[j][i] >= 65 && print->words[j][i] <= 90))
				{
					int x_on = ((print->words[j][i] - 65) % print->font_used->char_per_row) * fontsizex;
					int y_on = ((print->words[j][i] - 65) / print->font_used->char_per_row) * fontsizey;
					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale*1.3;
					fontsizex *= print->scale*1.3;

				}

				//characters a to z (97 - 122)
				else if (print->words[j][i] >= 97 && print->words[j][i] <= 122)
				{
					int x_on = ((print->words[j][i] - 97) % print->font_used->char_per_row) *fontsizex;
					int y_on = ((print->words[j][i] - 97) / print->font_used->char_per_row) * fontsizey;
					on_img = { x_on, y_on, fontsizex, fontsizey };

					fontsizey *= print->scale;
					fontsizex *= print->scale;

				}

				if (print->words[j][i] == 32)
				{
					length_so_far += print->font_used->hsize*print->scale * scale;
				}
				else
				{
					int x_scr = item->x*scale + length_so_far;
					int y_scr = item->y*scale - fontsizey + scale * y_level * print->scale*(print->font_used->vsize + 5);

					//x_scr *= scale;
					//y_scr *= scale;
					fontsizex *= scale;
					fontsizey *= scale;

					SDL_Rect on_screen = { x_scr,y_scr,fontsizex,fontsizey };

					length_so_far += on_screen.w;

					if (iterator < print->current_letter)
					{
						if (SDL_RenderCopyEx(renderer, item->to_print->font_used->font_texture, &on_img, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
						{
							std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
							errstr += SDL_GetError();
							Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
							ret = false;
						}
					}
					iterator++;

				}


			}


		}
		/*
		for (int i = 0; i < print->text.size(); i++)//-97
		{
			//CHECK FOR NEXT LINE

			int length_of_current_word=0;
			for (int j = 0; j < print->text.size(); ++j)
			{
				length_of_current_word += fontsizex*scale;
				if (print->text[j] == 32)
				{
					//printf("%d %c ",length_of_current_word,print->text[print->current_letter]);
					break;
				}
			}
			//if (length_so_far/scale > print->max_width)



			SDL_Rect on_img;

			//characters 0 to 9
			if (print->text[i] >= 48 && print->text[i] <= 57)
			{

				int x_on = print->font_used->hsize*(print->text[i] - 48);
				int y_on = print->font_used->hsize * 3;

				 on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale;
				fontsizex *= print->scale;


			}

			//character ,
			if (print->text[i] == 44)
			{
				int x_on = 0;
				int y_on = print->font_used->hsize * 4;

				 on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale;
				fontsizex *= print->scale;

			}

			//character !
			if (print->text[i] == 33)
			{
				int x_on = print->font_used->hsize * 1;
				int y_on = print->font_used->hsize * 4;

				 on_img = { x_on, y_on, fontsizex, fontsizey };

				 fontsizey *= print->scale;
				 fontsizex *= print->scale;
			}

			//character /
			if (print->text[i] == 47)
			{
				int x_on = print->font_used->hsize * 2;
				int y_on = print->font_used->hsize * 4;

				 on_img = { x_on, y_on, fontsizex, fontsizey };

				 fontsizey *= print->scale;
				 fontsizex *= print->scale;

			}


			//characters A to Z (65 - 90)
			if ((print->text[i] >= 65 && print->text[i] <= 90))
			{
				int x_on = ((print->text[i] - 65) % print->font_used->char_per_row) * fontsizex;
				int y_on = ((print->text[i] - 65) / print->font_used->char_per_row) * fontsizey;
				 on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale*1.3;
				fontsizex *= print->scale*1.3;

			}

			//characters a to z (97 - 122)
			else if (print->text[i] >= 97 && print->text[i] <= 122)
			{
				int x_on = ((print->text[i] - 97) % print->font_used->char_per_row) *fontsizex;
				int y_on = ((print->text[i] - 97) / print->font_used->char_per_row) * fontsizey;
				 on_img = { x_on, y_on, fontsizex, fontsizey };

				fontsizey *= print->scale;
				fontsizex *= print->scale;

			}
						
			if (print->text[i] == 32)
			{
				length_so_far += print->font_used->hsize*print->scale * scale;
			}
			else
			{
				int x_scr = item->x*scale + length_so_far;
				int y_scr = item->y*scale - fontsizey +scale* y_level * print->scale*(print->font_used->vsize + 5);
				
				//x_scr *= scale;
				//y_scr *= scale;
				fontsizex *= scale;
				fontsizey *= scale;

				SDL_Rect on_screen = { x_scr,y_scr,fontsizex,fontsizey };

				length_so_far += on_screen.w;

				if (i < print->current_letter)
				{
					if (SDL_RenderCopyEx(renderer, item->to_print->font_used->font_texture, &on_img, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
					{
						printf("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
						ret = false;
					}
				}
			}
			

			
		}
		*/delete item;
		text_queue.pop();
	}

	while (!quad_queue.empty())
	{
		BlitRect* item = quad_queue.front();
		float scale = App->win->GetScale();

		SDL_Rect temp = { item->area->x + App->cam->GetCameraXoffset(),item->area->y + App->cam->GetCameraYoffset() ,item->area->w,item->area->h };

		temp.x *= scale;
		temp.y *= scale;
		temp.w *= scale;
		temp.h *= scale;

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, item->r, item->g, item->b, item->a);
		int result = (item->filled) ? SDL_RenderFillRect(renderer, &temp) : SDL_RenderDrawRect(renderer,&temp);

		if (result != 0)
		{
			std::string errstr = "Cannot draw quad to screen. SDL_RenderFillRect error: ";
			errstr += SDL_GetError();
			Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			ret = false;
		}

		delete item;
		quad_queue.pop();
	}

	while (!trail_queue.empty())
	{
		Trail* item = trail_queue.front();
		float scale = App->win->GetScale();

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
			std::string errstr = "Cannot draw trail to screen. SDL_RenderFillRect error: ";
			errstr += SDL_GetError();
			Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			ret = false;
		}

		delete item;
		trail_queue.pop();
	}



	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return ret;
}

void Render::Blit(TextureID aTexID, int x, int y, SDL_Rect* rect_on_image, int depth, float angle, float parallax_factor_x, float parallax_factor_y, int center_x,int center_y)
{
	BlitItem* it = new BlitItem();
	it->depth = depth;
	it->on_img = rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = App->tex->Get_Texture(aTexID);

	it->center_x = center_x;
	it->center_y = center_y;

	//it->img_center = {center_x,center_y};
	it->angle = angle;
	it->parallax_factor_x = parallax_factor_x;
	it->parallax_factor_y = parallax_factor_y;

	blit_queue.push_back(it);
	
	//order the elements
	


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
void Render::BlitMapBackground(TextureID aTexID, int depth, bool repeat_y, float parallax_factor_x, float parallax_factor_y)
{
	BlitBackground* it = new BlitBackground();

	it->depth = depth;
	it->tex = App->tex->Get_Texture(aTexID);

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

void Render::BlitUI(TextureID aTexID, int x, int y, SDL_Rect* rect_on_image, int depth, float angle)
{
	BlitItem* it = new BlitItem();
	it->depth = depth;
	it->on_img = rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = App->tex->Get_Texture(aTexID);

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

	SDL_DestroyRenderer(renderer);

	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
	active = false;

	return ret;
}