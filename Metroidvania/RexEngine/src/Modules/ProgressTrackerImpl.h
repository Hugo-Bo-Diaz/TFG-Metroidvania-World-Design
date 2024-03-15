#ifndef PROGRESSTRACKER_IMPL__H
#define PROGRESSTRACKER_IMPL__H


#include "PartImpl.h"
#include "../include/Modules/ProgressTracker.h"
#include "pugiXML/src/pugixml.hpp"

class ProgressTracker::ProgressTrackerImpl : public Part::Part_Impl
{
public:

	ProgressTracker::ProgressTrackerImpl(ProgressTracker* aProgressTracker) :mPartInst(aProgressTracker){};

	void SetProgressTrackerInst(ProgressTracker* aProgressTracker)
	{
		mPartInst = aProgressTracker;
	}

	void SaveToNode(Section* aSector, pugi::xml_node& atr);
	void LoadFromNode(Section* aSector, pugi::xml_node& atr);

private:
	friend class ProgressTracker;

	ProgressTracker* mPartInst;
};

#endif