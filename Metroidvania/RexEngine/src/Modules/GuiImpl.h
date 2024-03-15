#ifndef GUI_IMPL__H
#define GUI_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Gui.h"

class UserInterface::GuiImpl : public Part::Part_Impl
{
public:

	UserInterface::GuiImpl(UserInterface* aAudio) : mPartInst(aAudio) {};

	void SetGuiInst(UserInterface* aAudio)
	{
		mPartInst = aAudio;
	}
	void RenderDebug();

protected:
	bool Init();
	bool Loop(float dt);
	bool CleanUp();
	void Clearelements();

	std::vector<UIelement*> elements;
	std::vector<UIelement*> to_delete;

private:
	friend class UserInterface;
	UserInterface* mPartInst;
};
#endif