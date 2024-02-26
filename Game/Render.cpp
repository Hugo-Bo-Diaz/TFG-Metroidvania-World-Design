#include "Render.h"
#include "Application.h"
#include "Window.h"
#include "Textures.h"
#include "Camera.h"
#include "Particles.h"
#include "ParticleEmitter.h"
#include "Text.h"
#include "SceneController.h"
#include "Logger.h"

#include <stdio.h>
#include <sstream>

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
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* Render::GetQueue(RenderQueue aQueue)
{
	switch (aQueue)
	{
	case RENDER_GAME:
		return &allQueue;
		break;
	case RENDER_UI:
		return &uiQueue;
		break;
	case RENDER_DEBUG:
		return &debugQueue;
		break;
	default:
		break;
	}

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

	for (int i = 0; i < RenderQueue::RENDER_MAX; i++)
	{
		std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* lQueue = GetQueue((RenderQueue)i);
		
		while (!lQueue->empty())
		{
			BlitItem* lNextItem = lQueue->top();
			lQueue->pop();

			lNextItem->Blit(this);
			delete lNextItem;
		}
	}

	//while (!debugQueue.empty())
	//{
	//	BlitItem* lNextItem = debugQueue.top();
	//	debugQueue.pop();

	//	lNextItem->Blit(this);
	//	delete lNextItem;
	//}


	//App->scn->RenderTiles();

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return ret;
}

void Render::Blit(TextureID aTexID, int x, int y, SDL_Rect* rect_on_image, int depth, RenderQueue aQueue, float angle, float parallax_factor_x, float parallax_factor_y, int center_x,int center_y)
{
	BlitTexture* it = new BlitTexture();
	it->depth = depth;
	it->on_image = *rect_on_image;
	it->x = x;
	it->y = y;
	it->tex = aTexID;

	it->ignore_camera = aQueue == RenderQueue::RENDER_UI;

	it->center_x = center_x;
	it->center_y = center_y;

	//it->img_center = {center_x,center_y};
	it->angle = angle;
	it->parallax_x = parallax_factor_x;
	it->parallax_y = parallax_factor_y;

	GetQueue(aQueue)->push(it);
	
	//order the elements
	


}

void Render::BlitMapLayer(layer* layer)
{
	BlitLayer* it = new BlitLayer();
	it->layer = layer;
	it->depth = layer->depth;
	allQueue.push(it);

}

void Render::BlitMapBackground(TextureID aTexID, int depth, bool repeat_y, float parallax_factor_x, float parallax_factor_y)
{
	BlitBackground* it = new BlitBackground();

	it->tex = aTexID;

	it->repeat_y = repeat_y;
	it->depth = depth;

	//it->img_center = {center_x,center_y};
	it->parallax_x = parallax_factor_x;
	it->parallax_y = parallax_factor_y;

	//order the elements
	allQueue.push(it);
}

void Render::BlitText(const char* text, FontID font, int x, int y, int depth, SDL_Color aColor, RenderQueue aQueue)
{
	BlitItemText* it = new BlitItemText();

	it->ignore_camera = aQueue == RenderQueue::RENDER_UI;
	it->x = x;
	it->y = y;
	it->color = aColor;
	it->font_used = font;
	it->mText = text;
	it->depth = depth;

	GetQueue(aQueue)->push(it);
}


void Render::DrawRect(SDL_Rect* area, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, RenderQueue aQueue)
{
	BlitRect* it = new BlitRect();
	it->x = area->x;
	it->y = area->y;
	it->w = area->w;
	it->h = area->h;
	it->color = SDL_Color{ r,g,b,a };
	it->filled = filled;
	it->ignore_camera = aQueue == RenderQueue::RENDER_UI;

	GetQueue(aQueue)->push(it);
}

void Render::DrawTrail(SDL_Point* point_array, int amount, RenderQueue aQueue, Uint8 r, Uint8 g, Uint8 b)
{
	BlitTrail* it = new BlitTrail();
	it->points = point_array;
	it->amount = amount;
	it->color = SDL_Color{ r,g,b };
	it->ignore_camera = aQueue == RenderQueue::RENDER_UI;
	GetQueue(aQueue)->push(it);
}

bool Render::CleanUp()
{
	bool ret = true;

	SDL_DestroyRenderer(renderer);

	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
	active = false;

	return ret;
}

void BlitTexture::Blit(Render* lRender)
{
	float scale = App->win->GetScale();

	SDL_Rect rect;
	SDL_Texture* lTex = App->tex->Get_Texture(tex);

	if (!ignore_camera)
	{
		rect.x = (float)x * scale + App->cam->GetCameraXoffset() * parallax_x;
		rect.y = (float)y * scale + App->cam->GetCameraYoffset() * parallax_y;
	}
	else
	{
		rect.x = (float)x * scale;
		rect.y = (float)y * scale;
	}

	rect.w = on_image.w * scale;
	rect.h = on_image.h * scale;

	SDL_Point p = { center_x,center_y };

	if (SDL_RenderCopyEx(lRender->renderer, lTex, &on_image, &rect, angle, &p, SDL_FLIP_NONE) != 0)
	{
		std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
		//ret = false;
	}
}

void BlitLayer::Blit(Render* lRender)
{
	float scale = App->win->GetScale();

	float para_x = layer->parallax_x;
	float para_y = layer->parallax_y;
	int depth = layer->depth;

	SDL_Texture* lTex = App->tex->Get_Texture(layer->tileset_of_layer->texture);
	tileset* t = layer->tileset_of_layer;//(*tilesets.begin());

	for (int _y = 0; _y < layer->height; ++_y)
	{
		for (int _x = 0; _x < layer->width; ++_x)
		{
			int i = _y * layer->width + _x;
			//tileset* t = GetTilesetFromId((*it)->data[i]);
			float worldcoords_x = scale * _x * t->tile_width + App->cam->GetCameraXoffset();
			float worldcoords_y = scale * _y * t->tile_height + App->cam->GetCameraYoffset();

			//App->ren->BlitMapTile(t->texture, _x, _y, GetImageRectFromId((*it)->data[i]), depth, para_x, para_y);

			SDL_Rect on_scn;
			on_scn.x = worldcoords_x;
			on_scn.y = worldcoords_y;
			on_scn.w = 48 * scale;
			on_scn.h = 48 * scale;

			if (SDL_RenderCopyEx(App->ren->renderer, lTex, &GetImageRectFromId(layer->tileset_of_layer, layer->data[i]), &on_scn, 0, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}
		}
	}

}

SDL_Rect BlitLayer::GetImageRectFromId(tileset* t, int id)
{

	SDL_Rect rect;
	rect.w = t->tile_width;
	rect.h = t->tile_height;
	rect.x = ((rect.w) * (id % t->columns));
	rect.y = ((rect.h) * (id / t->columns));

	return rect;
}


void BlitBackground::Blit(Render* lRender)
{
	int back_w, back_h;

	SDL_Texture* lTex = App->tex->Get_Texture(tex);

	SDL_QueryTexture(lTex, NULL, NULL, &back_w, &back_h);

	float scale = App->win->GetScale();

	//calculate camera tile

	int cam_tile_x;
	int cam_tile_y;
	int j = 0;
	bool exit = false;
	while (!exit)
	{
		if (App->cam->position_x * parallax_x >= (j - 1) * back_w && App->cam->position_x * parallax_x < j * back_w)
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
		if (App->cam->position_y * parallax_y >= (j - 1) * back_h && App->cam->position_y * parallax_y < j * back_h)
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

			int pos_x = ((cam_tile_x - 1 + i) * back_w) * scale;
			int pos_y = ((cam_tile_y - 1 + j) * back_h) * scale;

			rect.x = pos_x + App->cam->GetCameraXoffset() * scale * parallax_x;
			rect.y = pos_y + App->cam->GetCameraYoffset() * scale * parallax_y;

			if (!repeat_y)
			{
				rect.y = 0;// App->scn->room_h * 48 - App->cam->height;
			}

			rect.w = back_w;
			rect.h = back_h;

			rect.w *= scale;
			rect.h *= scale;

			if (SDL_RenderCopyEx(lRender->renderer, lTex, NULL, &rect, 0, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}

			if (!repeat_y)
			{
				break;
			}
		}
	}
}

void BlitRect::Blit(Render* lRender)
{
	float scale = App->win->GetScale();

	SDL_Rect temp;

	if (!ignore_camera)
	{
		temp = { x + (int)App->cam->GetCameraXoffset(),y + (int)App->cam->GetCameraYoffset() ,w,h };
	}
	else
	{
		temp = { x,y,w,h };
	}

	temp.x *= scale;
	temp.y *= scale;
	temp.w *= scale;
	temp.h *= scale;
	SDL_SetRenderDrawBlendMode(lRender->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(lRender->renderer, color.r, color.g, color.b, color.a);
	//SDL_SetRenderDrawColor(lRender->renderer, color.r, color.g, color.b, 255);
	int result = (filled) ? SDL_RenderFillRect(lRender->renderer, &temp) : SDL_RenderDrawRect(lRender->renderer, &temp);

	if (result != 0)
	{
		std::string errstr = "Cannot draw quad to screen. SDL_RenderFillRect error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
	}
}

void BlitTrail::Blit(Render* lRender)
{
	float scale = App->win->GetScale();

	if (!ignore_camera)
	{
		for (int i = 0; i < amount; ++i)
		{
			points[i].x += App->cam->GetCameraXoffset();
			points[i].y += App->cam->GetCameraYoffset();
		}
	}

	SDL_SetRenderDrawBlendMode(lRender->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(lRender->renderer, color.r, color.g, color.b, 255);// it's a debug feature so it'll have max visibility
	int result = SDL_RenderDrawLines(lRender->renderer, points, amount);

	if (!ignore_camera)
	{
		for (int i = 0; i < amount; ++i)
		{
			points[i].x -= App->cam->GetCameraXoffset();
			points[i].y -= App->cam->GetCameraYoffset();
		}
	}

	if (result != 0)
	{
		std::string errstr = "Cannot draw trail to screen. SDL_RenderFillRect error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
	}
}

void BlitParticles::Blit(Render* lRender)
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (lEmmitter->particles[i] != nullptr)
		{
			float scale = App->win->GetScale();

			SDL_Rect rect;
			rect.x = lEmmitter->particles[i]->target_on_screen.x * scale + App->cam->GetCameraXoffset();
			rect.y = lEmmitter->particles[i]->target_on_screen.y * scale + App->cam->GetCameraYoffset();


			rect.w = lEmmitter->particles[i]->target_on_screen.w * lEmmitter->particles[i]->current_scale;
			rect.h = lEmmitter->particles[i]->target_on_screen.h * lEmmitter->particles[i]->current_scale;

			rect.x -= rect.w / 2;
			rect.y -= rect.h / 2;

			rect.w *= scale;
			rect.h *= scale;

			if (SDL_RenderCopyEx(lRender->renderer, lEmmitter->particles[i]->texture, lEmmitter->particles[i]->area_in_texture, &rect, lEmmitter->particles[i]->angle, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}
		}
	}
}

void BlitItemText::Blit(Render* lRender)
{
	// variable to store token obtained from the original
	int length_so_far = 0;
	int yLevel = 0;
	Font* lFont = App->txt->GetFont(font_used);

	for (int i = 0; i < mText.size(); ++i)
	{
		if(mText[i] == '\n')
		{
			yLevel += lFont->size*1.5;
			length_so_far = 0;
		}
		else if (mText[i] == ' ')
		{
			length_so_far += lFont->size/2;
		}
		else if (lFont->lMapping.count(mText[i]))
		{
			SDL_Rect* mappedRect = lFont->lMapping[mText[i]];
			SDL_Rect on_screen = SDL_Rect{ x + length_so_far,y + yLevel, mappedRect->w, mappedRect->h };

			length_so_far += on_screen.w;

			if (SDL_RenderCopyEx(lRender->renderer, lFont->font_texture, mappedRect, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}
		}
	}
}
