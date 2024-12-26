#include "FirstDialogue.h"
#include "../UIElements/UItextbox.h"
#include "Application.h"
#include "Modules/Gui.h"
#include "../SceneProcessing.h"
#include "Modules/Camera.h"

FirstDialogue::FirstDialogue()
{

}

void FirstDialogue::Init()
{
	texture = Engine->GetModule<Textures>().Load_Texture("Assets/UI/black_square.png");

	textbox = new UItextbox("", "For those who seek power are blind to their surroundings", TextBoxColor::GREY, 15, 4, 272, 160, 2, 0.2);
	textbox->AddPanelToTextBox("only when it is able to serve all will it be balanced");
	textbox->AddPanelToTextBox("Never forget though never stop moving forward seeking to repare one's mistakes");
	Engine->GetModule<UserInterface>().AddElement(textbox);
}

bool FirstDialogue::Loop(float dt)
{
	if (!Engine->GetModule<UserInterface>().ElementExists(textbox))
	{
		Engine->GetModule<ObjectManager>().DeleteObject(this);
		MetroidVaniaSceneProcessor::GetInstance().canopenmenu = true;
	}

	return true;
}

bool FirstDialogue::Render()
{
	Engine->GetModule<::Render>().Blit(texture, 0,0, nullptr,10, RenderQueue::RENDER_UI);

	return true;
}