#include "Modules/Render.h"
#include "Application.h"
#include "Modules/Window.h"
#include "Modules/Camera.h"
#include "Modules/Particles.h"
#include "EngineElements/ParticleEmitter.h"
#include "Modules/Text.h"
#include "Utils/Logger.h"
#include "TextImpl.h"
#include <stdio.h>
#include <sstream>
#include "RenderImpl.h"
#include "TexturesImpl.h"
#include "TextImpl.h"
#include "WindowImpl.h"
#include "EngineAPI.h"

Render::Render(EngineAPI& aAPI) :Part("Render",aAPI)
{ 
	mPartFuncts = new RenderImpl(this);
}

#pragma region IMPLEMENTATION

bool Render::RenderImpl::LoadConfig(pugi::xml_node& config_node)
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
	renderer = SDL_CreateRenderer(mPartInst->mApp.GetImplementation<Window,Window::WindowImpl>()->GetSDLWindow(), -1, flags);
	if(!renderer)
	{
		ret = false;
	}
	return ret;
}

bool Render::RenderImpl::CreateConfig(pugi::xml_node& config_node)
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


std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* Render::RenderImpl::GetQueue(RenderQueue aQueue)
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

bool Render::RenderImpl::Init()
{
	bool ret = true;

	return ret;
}

bool Render::RenderImpl::Loop(float dt)
{
	bool ret = true;
	SDL_RenderClear(renderer);

	mPartInst->mApp.GetModule<Camera>().screenarea.x = mPartInst->mApp.GetModule<Camera>().position_x;
	mPartInst->mApp.GetModule<Camera>().screenarea.y = mPartInst->mApp.GetModule<Camera>().position_y;

	mDrawCallsLastFrame = 0;

	for (int i = 0; i < RenderQueue::RENDER_MAX; i++)
	{
		std::priority_queue<BlitItem*, std::vector<BlitItem*>, Comparer>* lQueue = GetQueue((RenderQueue)i);
		
		while (!lQueue->empty())
		{
			BlitItem* lNextItem = lQueue->top();
			lQueue->pop();

			lNextItem->Blit(*mPartInst, mPartInst->mApp.GetModule<Camera>(), mPartInst->mApp.GetModule<Window>());
			delete lNextItem;
		}
	}


	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return ret;
}


void Render::RenderImpl::BlitMapLayer(layer* layer)
{
	BlitLayer* it = new BlitLayer();
	it->layer = layer;
	it->depth = layer->depth;
	it->tex = mPartInst->mApp.GetImplementation<Textures, Textures::TexturesImpl>()->Get_Texture(layer->tileset_of_layer->texture);
	allQueue.push(it);
}

void Render::RenderImpl::BlitParticleEmitter(ParticleEmitter* layer, RenderQueue aRenderQueue)
{
	BlitParticles* it = new BlitParticles();
	it->lEmmitter = layer;
	it->depth = layer->depth;
	it->tex = mPartInst->mApp.GetImplementation<Textures, Textures::TexturesImpl>()->Get_Texture(layer->preset_for_emitter->texture_name);
	GetQueue(aRenderQueue)->push(it);
}


void Render::RenderImpl::BlitMapBackground(TextureID aTexID, int depth, bool repeat_y, float parallax_factor_x, float parallax_factor_y)
{
	BlitBackground* it = new BlitBackground();

	it->tex = mPartInst->mApp.GetImplementation<Textures, Textures::TexturesImpl>()->Get_Texture(aTexID);

	it->repeat_y = repeat_y;
	it->depth = depth;

	//it->img_center = {center_x,center_y};
	it->parallax_x = parallax_factor_x;
	it->parallax_y = parallax_factor_y;

	//order the elements
	allQueue.push(it);
}

bool Render::RenderImpl::CleanUp()
{
	bool ret = true;

	SDL_DestroyRenderer(renderer);

	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
	return ret;
}

#pragma endregion

#pragma region PUBLIC API

long Render::GetDrawCallsLastFrame() 
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}
	return lImpl->mDrawCallsLastFrame;
}

void Render::CountDrawCall()
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
	}
	lImpl->mDrawCallsLastFrame++;
}


void Render::Blit(TextureID aTexID, int x, int y,const RXRect& rect_on_image, int depth, RenderQueue aQueue, float angle, float parallax_factor_x, float parallax_factor_y, int center_x,int center_y)
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	BlitTexture* it = new BlitTexture();
	it->depth = depth;
	it->on_image = { rect_on_image.x,rect_on_image.y,rect_on_image.w,rect_on_image.h };
	it->x = x;
	it->y = y;
	it->tex = mApp.GetImplementation<Textures,Textures::TexturesImpl>()->Get_Texture(aTexID);

	it->ignore_camera = aQueue == RenderQueue::RENDER_UI;

	it->center_x = center_x;
	it->center_y = center_y;

	it->angle = angle;
	it->parallax_x = parallax_factor_x;
	it->parallax_y = parallax_factor_y;

	lImpl->GetQueue(aQueue)->push(it);
}

void Render::BlitText(const char* text, FontID font, int x, int y, int depth, const RXColor& aColor, RenderQueue aQueue, bool ignore_camera)
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	BlitItemText* it = new BlitItemText();

	it->ignore_camera = aQueue == RenderQueue::RENDER_UI || ignore_camera;
	it->x = x;
	it->y = y;
	it->color = { aColor.r, aColor.g, aColor.b, aColor.a };
	it->font_used = mApp.GetImplementation<Text, Text::TextImpl>()->GetFont(font);
	if (it->font_used != nullptr)
	{
		it->lFontTexture = mApp.GetImplementation<Textures, Textures::TexturesImpl>()->Get_Texture(it->font_used->font_texture);
		it->mText = text;
		it->depth = depth;

		lImpl->GetQueue(aQueue)->push(it);
	}

}

void Render::DrawRect(const RXRect& area, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled, RenderQueue aQueue,int depth , bool ignore_camera)
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	BlitRect* it = new BlitRect();
	it->x = area.x;
	it->y = area.y;
	it->w = area.w;
	it->h = area.h;
	it->color = RXColor{ r,g,b,a };
	it->filled = filled;
	it->ignore_camera = aQueue == RenderQueue::RENDER_UI || ignore_camera;
	it->depth = depth;

	lImpl->GetQueue(aQueue)->push(it);
}

void Render::DrawTrail(RXPoint* point_array, int amount, RenderQueue aQueue,bool aIgnoreCamera,int depth, uint8_t r, uint8_t g, uint8_t b)
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return;
	}

	BlitTrail* it = new BlitTrail();

	SDL_Point* lPoints = new SDL_Point[amount];

	for (int i = 0; i < amount; ++i)
	{
		lPoints[i].x = point_array[i].x;
		lPoints[i].y = point_array[i].y;
	}

	it->points = lPoints;
	it->amount = amount;
	it->depth = depth;
	it->color = RXColor{ r,g,b ,255};
	it->ignore_camera = aQueue == RenderQueue::RENDER_UI || aIgnoreCamera;
	lImpl->GetQueue(aQueue)->push(it);
}

void BlitTexture::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	float scale = aWindow.GetScale();

	SDL_Rect rect;

	if (!ignore_camera)
	{
		rect.x = (float)x * scale + camera.GetCameraXoffset() * parallax_x;
		rect.y = (float)y * scale + camera.GetCameraYoffset() * parallax_y;
	}
	else
	{
		rect.x = (float)x * scale;
		rect.y = (float)y * scale;
	}

	rect.w = on_image.w * scale;
	rect.h = on_image.h * scale;

	SDL_Point p = { center_x,center_y };
	
	RXRect lRect = { rect.x,rect.y,rect.w,rect.h };
	if (!camera.isOnScreen(lRect, false))
		return;

	aRender.CountDrawCall();
	if (SDL_RenderCopyEx(aRender.GetSDL_Renderer(), tex, &on_image, &rect, angle, &p, SDL_FLIP_NONE) != 0)
	{
		std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
		//ret = false;
	}
}

void BlitLayer::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	float scale = aWindow.GetScale();

	float para_x = layer->parallax_x;
	float para_y = layer->parallax_y;
	int depth = layer->depth;

	tileset* t = layer->tileset_of_layer;//(*tilesets.begin());

	for (int _y = 0; _y < layer->height; ++_y)
	{
		for (int _x = 0; _x < layer->width; ++_x)
		{
			int i = _y * layer->width + _x;
			//tileset* t = GetTilesetFromId((*it)->data[i]);
			float worldcoords_x = scale * _x * t->tile_width + camera.GetCameraXoffset();
			float worldcoords_y = scale * _y * t->tile_height + camera.GetCameraYoffset();

			//App->ren->BlitMapTile(t->texture, _x, _y, GetImageRectFromId((*it)->data[i]), depth, para_x, para_y);

			SDL_Rect on_scn;
			on_scn.x = worldcoords_x;
			on_scn.y = worldcoords_y;
			on_scn.w = 48 * scale;
			on_scn.h = 48 * scale;

			RXRect lRect = { on_scn.x,on_scn.y,on_scn.w,on_scn.h };
			if (!camera.isOnScreen(lRect, false))
				continue;

			aRender.CountDrawCall();
			if (SDL_RenderCopyEx(aRender.GetSDL_Renderer(), tex, &GetImageRectFromId(layer->tileset_of_layer, layer->data[i]), &on_scn, 0, NULL, SDL_FLIP_NONE) != 0)
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

void BlitBackground::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	int back_w, back_h;

	SDL_QueryTexture(tex, NULL, NULL, &back_w, &back_h);

	float scale = aWindow.GetScale();
	//calculate camera tile

	int cam_tile_x;
	int cam_tile_y;
	int j = 0;
	bool exit = false;
	while (!exit)
	{
		if (camera.position_x * parallax_x >= (j - 1) * back_w && camera.position_x * parallax_x < j * back_w)
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
		if (camera.position_y * parallax_y >= (j - 1) * back_h && camera.position_y * parallax_y < j * back_h)
		{
			exit = true;
			cam_tile_y = j;
		}
		j++;
	}

	int rel_x = camera.position_x - cam_tile_x * back_w;
	int rel_y = camera.position_y - cam_tile_y * back_h;

	float images_that_fit_in_cam_x = camera.width / back_w;
	float images_that_fit_in_cam_y = camera.height / back_h;

	for (int i = 0; i <= images_that_fit_in_cam_x; ++i)
	{
		for (int j = 0; j <= images_that_fit_in_cam_y; ++j)
		{
			SDL_Rect rect;//DUAL LOOP THIS

			int pos_x = ((cam_tile_x - 1 + i) * back_w) * scale;
			int pos_y = ((cam_tile_y - 1 + j) * back_h) * scale;

			rect.x = pos_x + camera.GetCameraXoffset() * scale * parallax_x;
			rect.y = pos_y + camera.GetCameraYoffset() * scale * parallax_y;

			if (!repeat_y)
			{
				rect.y = 0;// App->scn->room_h * 48 - App->cam->height;
			}

			rect.w = back_w;
			rect.h = back_h;

			rect.w *= scale;
			rect.h *= scale;

			aRender.CountDrawCall();
			if (SDL_RenderCopyEx(aRender.GetSDL_Renderer(), tex, NULL, &rect, 0, NULL, SDL_FLIP_NONE) != 0)
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

void BlitRect::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	float scale = aWindow.GetScale();

	SDL_Rect temp;

	if (!ignore_camera)
	{
		temp = { x + (int)camera.GetCameraXoffset(),y + (int)camera.GetCameraYoffset() ,w,h };
	}
	else
	{
		temp = { x,y,w,h };
	}

	temp.x *= scale;
	temp.y *= scale;
	temp.w *= scale;
	temp.h *= scale;

	RXRect lRect = { temp.x,temp.y,temp.w,temp.h };
	if (!camera.isOnScreen(lRect, false))


	SDL_SetRenderDrawBlendMode(aRender.GetSDL_Renderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(aRender.GetSDL_Renderer(), color.r, color.g, color.b, color.a);
	//SDL_SetRenderDrawColor(lRender->renderer, color.r, color.g, color.b, 255);

	aRender.CountDrawCall();
	int result = (filled) ? SDL_RenderFillRect(aRender.GetSDL_Renderer(), &temp) : SDL_RenderDrawRect(aRender.GetSDL_Renderer(), &temp);

	if (result != 0)
	{
		std::string errstr = "Cannot draw quad to screen. SDL_RenderFillRect error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
	}
}

void BlitTrail::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	float scale = aWindow.GetScale();

	if (!ignore_camera)
	{
		for (int i = 0; i < amount; ++i)
		{
			points[i].x += camera.GetCameraXoffset();
			points[i].y += camera.GetCameraYoffset();
		}
	}

	SDL_SetRenderDrawBlendMode(aRender.GetSDL_Renderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(aRender.GetSDL_Renderer(), color.r, color.g, color.b, 255);// it's a debug feature so it'll have max visibility
	int result = SDL_RenderDrawLines(aRender.GetSDL_Renderer(), points, amount);

	delete points;

	if (result != 0)
	{
		std::string errstr = "Cannot draw trail to screen. SDL_RenderFillRect error: ";
		errstr += SDL_GetError();
		Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
	}
}

void BlitParticles::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	float scale = aWindow.GetScale();
	
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (lEmmitter->particles[i] != nullptr)
		{
			SDL_Rect rect;
			rect.x = lEmmitter->particles[i]->target_on_screen.x * scale + camera.GetCameraXoffset();
			rect.y = lEmmitter->particles[i]->target_on_screen.y * scale + camera.GetCameraYoffset();

			rect.w = lEmmitter->particles[i]->target_on_screen.w * lEmmitter->particles[i]->current_scale;
			rect.h = lEmmitter->particles[i]->target_on_screen.h * lEmmitter->particles[i]->current_scale;

			rect.x -= rect.w / 2;
			rect.y -= rect.h / 2;

			rect.w *= scale;
			rect.h *= scale;

			RXRect* lRecFromEmitter = lEmmitter->particles[i]->area_in_texture;
			SDL_Rect lRectInText = {lRecFromEmitter->x, lRecFromEmitter->y, lRecFromEmitter->w, lRecFromEmitter->h};

			RXRect lRect = { rect.x,rect.y,rect.w,rect.h };
			if (!camera.isOnScreen(lRect, false))
				continue;

			aRender.CountDrawCall();
			if (SDL_RenderCopyEx(aRender.GetSDL_Renderer(), tex, &lRectInText, &rect, lEmmitter->particles[i]->angle, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}
		}
	}
}

SDL_Renderer* Render::GetSDL_Renderer()
{
	RenderImpl* lImpl = dynamic_cast<RenderImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return nullptr;
	}
	return lImpl->renderer;
}

void BlitItemText::Blit(Render& aRender, Camera& camera, Window& aWindow)
{
	// variable to store token obtained from the original
	int length_so_far = 0;
	int yLevel = 0;

	if (font_used == nullptr)
		return;

	for (int i = 0; i < mText.size(); ++i)
	{
		if(mText[i] == '\n')
		{
			yLevel += font_used->size*1.5;
			length_so_far = 0;
		}
		else if (mText[i] == ' ')
		{
			length_so_far += font_used->size/2;
		}
		else if (font_used->lMapping.count(mText[i]))
		{
			SDL_Rect* mappedRect = font_used->lMapping[mText[i]];
			SDL_Rect on_screen = SDL_Rect{ x + length_so_far,y + yLevel, mappedRect->w, mappedRect->h };

			length_so_far += on_screen.w;

			RXRect lRect = { on_screen.x,on_screen.y,on_screen.w,on_screen.h };
			if (!camera.isOnScreen(lRect,false))
				continue;

			aRender.CountDrawCall();
			if (SDL_RenderCopyEx(aRender.GetSDL_Renderer(), lFontTexture, mappedRect, &on_screen, 0, NULL, SDL_FLIP_NONE) != 0)
			{
				std::string errstr = "Cannot blit to screen. SDL_RenderCopy error: ";
				errstr += SDL_GetError();
				Logger::Console_log(LogLevel::LOG_ERROR, errstr.c_str());
			}
		}
	}
}

#pragma endregion