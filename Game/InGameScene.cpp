#include "InGameScene.h"

#include "Application.h"
#include "Textures.h"
#include "Render.h"
#include "Physics.h"
#include "Input.h"
#include "SceneController.h"

bool InGameScene::Create()
{
	printf("Creating InGame Scene\n");
	App->tex->Load_Texture("Assets/Backgrounds/ingamebackground.png","background");

	//Wall_limit_left = new SDL_Rect({ 0, 0, 64, 720 });
	////*Wall_limit_left =;
	//Wall_limit_right = new SDL_Rect({ 1216, 0, 64, 720 });
	////*Wall_limit_right = ;
	//Floor_left = new SDL_Rect({ 192, 576, 384, 64 });
	////*Floor_left = ;
	//Floor_right = new SDL_Rect({ 704, 576, 384, 64 });
	////*Floor_right = ;

	//Middle_platform = new SDL_Rect({ 576, 448, 128, 64 });
	////*Middle_platform = ;

	//Left_platform = new SDL_Rect({ 192, 384, 254, 64 });
	////*Left_platform = ;
	//Right_platform = new SDL_Rect({ 832, 384, 254, 64 });
	////*Right_platform = ;

	//Ceiling = new SDL_Rect({ 64, 0, 1152, 128 });
	//*Ceiling = ;

	Wall_limit_left = { 0, 0, 64, 720 };
	//*Wall_limit_left =;
	Wall_limit_right ={ 1216, 0, 64, 720 };
	//*Wall_limit_right = ;
	Floor_left = { 192, 576, 384, 64 };
	//*Floor_left = ;
	Floor_right = { 704, 576, 384, 64 };
	//*Floor_right = ;
	
	Middle_platform = { 576, 448, 128, 64 };
	//*Middle_platform = ;

	Left_platform = { 192, 384, 254, 64 };
	//*Left_platform = ;
	Right_platform = { 832, 384, 254, 64 };
	//*Right_platform = ;

	Ceiling = { 64, 0, 1152, 128 };

	return true;
}

bool InGameScene::Start()
{

	App->phy->AddWall(Wall_limit_left);
	App->phy->AddWall(Wall_limit_right);
	App->phy->AddWall(Floor_left);
	App->phy->AddWall(Floor_right);
	App->phy->AddWall(Middle_platform);
	App->phy->AddWall(Left_platform);
	App->phy->AddWall(Right_platform);
	App->phy->AddWall(Ceiling);

	App->phy->AddObject(600, 360, 64, 64, PLAYER);
	return true;
}
bool InGameScene::Loop()
{
	App->ren->Blit(App->tex->Get_Texture("background"), 0, 0, 0, 100);

	return true;
}
bool InGameScene::Stop()
{
	App->phy->Clearphysics();
	return true;
}

bool InGameScene::Destroy()
{
	if (App->tex->Valid_Texture(""))
	{
		App->tex->Destroy_Texture("background");
	}
	return true;
}