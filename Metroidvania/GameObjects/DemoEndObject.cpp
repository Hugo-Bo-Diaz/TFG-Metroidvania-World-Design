#include "DemoEndObject.h"
#include "Input.h"
//#include "../UIelementFunctions.h"
#include "../UIElements/UItextbox.h"
#include "ProgressTracker.h"
#include "Player.h"
#include "Camera.h"

EndDemoObject::EndDemoObject()
{
	indicator = App->tex->Load_Texture("Assets/UI/indicator.png");
	particles = App->tex->Load_Texture("Assets/Sprites/particles.png");

	rectanglekeyboard = { 0,0,48,48 };
	rectanglecontroller = { 48,0,48,48 };
}

bool EndDemoObject::Loop(float dt)
{
	bool iscontactingplayer = false;

	std::vector<collision*> collisions;
	App->phy->GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				iscontactingplayer = true;
				if (App->inp->GetInput(BUTTON_2) == BUTTON_DOWN)
				{
					playerbooks = ((Player*)(*it)->object)->unlocked_spells;
					std::string s1 = "You have " + std::to_string(playerbooks) + "/2 books required to complete the demo" ;
					
					std::string s2 = "";
					if (playerbooks >= 2)
						s2 = "Congratulations you have completed the demo, the game will exit...";
					else
						s2 = "keep searching the books are still there!";

					textbox = new UItextbox(author.c_str(), s1.c_str(), c, 15, 4, 272, 420, 2, 0.2);
					textbox->AddPanelToTextBox(s2.c_str());
					App->gui->AddElement(textbox);

					textboxexists = true;
				}

			}
		}
	}

	player_contact = iscontactingplayer;

	if (textboxexists && !App->gui->ElementExists(textbox))
	{
		textboxexists = false;
		//App->trk->pl->speed_y = 0;
		if (playerbooks >= 2)
		{
			//END DEMO HERE :D
			App->phy->UnPauseObjects();
			//App->trk->go_to_main_menu = true;
			App->trk->SaveGame("save_file.xml");
			App->cam->CoverScreen(1000, 300, 0, 0, 0);
		}
	}

	return true;
}

bool EndDemoObject::Render()
{
	if (player_contact == true)
	{
		if (App->inp->using_controller)
			App->ren->Blit(indicator, collider->x + collider->w / 2 - 24, collider->y - 48, &rectanglecontroller, -5);
		else
			App->ren->Blit(indicator, collider->x + collider->w / 2 - 24, collider->y - 48, &rectanglekeyboard, -5);
	}

	return true;
}


EndDemoObject::~EndDemoObject()
{
}

void EndDemoObject::AddText(const char * text)
{
	strings.push_back(text);
}
