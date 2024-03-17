#ifndef GUI_IMPL__H
#define GUI_IMPL__H

#include "PartImpl.h"
#include "../include/Modules/Gui.h"
#include <unordered_set>

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
	bool Loop(float dt);
	bool CleanUp();
	void Clearelements();

private:
	friend class UserInterface;
	UserInterface* mPartInst;

	std::vector<UIelement*> elements;
	std::unordered_set<UIelement*> to_delete;
};
#endif