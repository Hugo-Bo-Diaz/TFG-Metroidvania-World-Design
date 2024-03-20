#include "TextBoxObject.h"
#include "Modules/Input.h"
#include "../UIElements/UItextbox.h"
#include "Modules/ProgressTracker.h"
#include "Player.h"
#include "../UIelementFunctions.h"
#include "../SceneProcessing.h"

TextBoxObject::TextBoxObject()
{

}

void TextBoxObject::Init()
{
	texture = Engine->GetModule<Textures>().Load_Texture("Assets/UI/indicator.png");

	rectanglekeyboard = {0,0,48,48};
	rectanglecontroller = { 48,0,48,48 };
	if (Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs") == nullptr)
	{
		Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->AddNewChild("LoreLogs");
	}
}

TextBoxObject::TextBoxObject(std::list<ObjectProperty*>& aProperties)
{
	new (this) TextBoxObject;

	for (std::list<ObjectProperty*>::iterator it = aProperties.begin(); it != aProperties.end(); ++it)
	{
		if ((*it)->name.compare("text") == 0)
		{
			AddText((*it)->str_value.c_str());
		}
		else if ((*it)->name.compare("author") == 0)
		{
			author = (*it)->str_value;
		}
		else if ((*it)->name.compare("lore") == 0)
		{
			lore_unlock = (*it)->num_value;
		}
	}

}


bool TextBoxObject::Loop(float dt)
{
	bool iscontactingplayer = false;

	std::vector<collision*> collisions;
	Engine->GetModule<ObjectManager>().GetCollisions(collider, collisions);

	for (std::vector<collision*>::iterator it = collisions.begin(); it != collisions.end(); it++)
	{
		if ((*it)->object != this)
		{
			if ((*it)->object->IsSameTypeAs<Player>())
			{
				iscontactingplayer = true;
				if (Engine->GetModule<Input>().GetInput(BUTTON_2) == BUTTON_DOWN && strings.size()>0)
				{
					textbox = new UItextbox(author.c_str(), strings[0].c_str(), c, 15, 4, 272, 420, 2, 0.2);
					Engine->GetModule<UserInterface>().AddElement(textbox);

					int max_text = strings.size();
					if (max_text > 1)
					{
						for (int i = 1; i < max_text; ++i)
						{
							textbox->AddPanelToTextBox(strings[i].c_str());
						}
					}
					textboxexists = true;

					if (lore_unlock != -1 && Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->GetValue(std::to_string(lore_unlock).c_str()) == 0.0f)
					{
						Engine->GetModule<ProgressTracker>().GetBaseSaveSection()->GetChild("LoreLogs")->SetValue(std::to_string(lore_unlock).c_str(), 1.0f);
						textbox->AddPanelToTextBox("New lore entry unlocked");
					}
				}

			}
		}
	}
	
	player_contact = iscontactingplayer;

	if (textboxexists && !Engine->GetModule<UserInterface>().ElementExists(textbox))
	{
		textboxexists = false;
		MetroidVaniaSceneProcessor::GetInstance().pl->speed_y = 0;

	}

	return true;
}

bool TextBoxObject::Render()
{
	if (player_contact == true)
	{
		if(Engine->GetModule<Input>().IsUsingController())
			Engine->GetModule<::Render>().RenderTexture(texture, collider->x + collider->w/2 - 24, collider->y - 48, rectanglecontroller,-5);
		else
			Engine->GetModule<::Render>().RenderTexture(texture, collider->x + collider->w / 2 - 24, collider->y - 48, rectanglekeyboard, -5);
	}

	return true;
}


TextBoxObject::~TextBoxObject()
{
}

void TextBoxObject::AddText(const char * text)
{
	strings.push_back(text);
}
