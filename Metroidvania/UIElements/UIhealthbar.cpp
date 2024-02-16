#include "UIhealthbar.h"
#include "Render.h"
#include "Application.h"

UIhealthbar::UIhealthbar(playerresource resource)
{
	res = resource;

	begin = { 0,0,size_in_image,size_in_image };
	middle = { size_in_image,0,size_in_image,size_in_image };
	end = { size_in_image*2,0,size_in_image,size_in_image };
	bar = {size_in_image*3, 0, size_in_image, size_in_image };

	last_bar = bar;


	if (res == HEALTH)
	{
		begin.y+= size_in_image;
		middle.y+= size_in_image;
		end.y += size_in_image;
		bar.y += size_in_image;
	}

	TextureHB = App->tex->Load_Texture("Assets/UI/healthandmana.png");
}

void UIhealthbar::Loop()
{
	UIelement::Loop();

	if (player == nullptr)
		return;

	switch (res)
	{
	case HEALTH:
		curr_res = player->health;
		max_res = player->max_health;
		break;
	case MANA:
		curr_res = player->mana;
		max_res = player->max_mana;
		break;
	default:
		break;
	}


}

void UIhealthbar::Render()
{
	UIelement::Render();
	//background
	//begin section

	if (player == nullptr)
		return;

	App->ren->BlitUI(TextureHB, x, y, &begin, 30);
	//middle sections
	int offset = size_in_image;
	for (int i = 0; i < max_res; ++i)
	{
		App->ren->BlitUI(TextureHB, x+offset, y, &middle, 30);
		offset += size_in_image;
	}

	//final sections
	App->ren->BlitUI(TextureHB, x+offset, y, &end, 30);

	//bars
	offset = size_in_image;
	for (int i = 0; i < curr_res; ++i)
	{
		App->ren->BlitUI(TextureHB, x + offset, y, &bar, 25);
		offset += size_in_image;
	}
	
	if (res == MANA)//if it is mana it should a last partial section
	{
		//bar.w = max length
		//player.mana- curr res = 
		float realw = (player->mana - curr_res) * bar.w;
		last_bar.w = realw;//gives a value from 0 to 1 because of the size of the fragments
		App->ren->BlitUI(TextureHB, x + offset, y, &last_bar, 25);
	}
}
