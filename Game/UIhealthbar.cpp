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

	if (res == HEALTH)
	{
		begin.y+= size_in_image;
		middle.y+= size_in_image;
		end.y += size_in_image;
		bar.y += size_in_image;
	}

}

void UIhealthbar::Loop()
{
	UIelement::Loop();

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

	App->ren->BlitUI(App->tex->Get_Texture("healthbars"), x, y, &begin, 30);
	//middle sections
	int offset = size_in_image;
	for (int i = 0; i < max_res; ++i)
	{
		App->ren->BlitUI(App->tex->Get_Texture("healthbars"), x+offset, y, &middle, 30);
		offset += size_in_image;
	}

	//final sections
	App->ren->BlitUI(App->tex->Get_Texture("healthbars"), x+offset, y, &end, 30);

	//bars
	offset = size_in_image;
	for (int i = 0; i < curr_res; ++i)
	{
		App->ren->BlitUI(App->tex->Get_Texture("healthbars"), x + offset, y, &bar, 25);
		offset += size_in_image;
	}
}
