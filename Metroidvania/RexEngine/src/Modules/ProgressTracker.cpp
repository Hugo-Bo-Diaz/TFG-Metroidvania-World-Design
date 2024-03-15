#include "Modules/ProgressTracker.h"
#include "Application.h"

#include "ProgressTrackerImpl.h"

#include <string>
#include <sstream>
#include "Utils/Logger.h"

ProgressTracker::ProgressTracker(EngineAPI& aAPI) : Part("ProgressTracker",aAPI)
{
	mPartFuncts = new ProgressTrackerImpl(this);

	BaseSaveSection = new Section();
	BaseSettingsSection = new Section();
}

#pragma region IMPLEMENTATION

void ProgressTracker::ProgressTrackerImpl::SaveToNode(Section* aSector, pugi::xml_node& aNode)
{
	pugi::xml_node lValsNode = aNode.append_child("ValuesNode");

	for (auto i = aSector->Values.begin(); i != aSector->Values.end(); i++)
	{
		//add values to node
		pugi::xml_node lNode = lValsNode.append_child(i->first.c_str());
		lNode.text().set(std::to_string(i->second).c_str());
	}

	pugi::xml_node lNode = aNode.append_child("ChildrenNode");

	for (auto i = aSector->Children.begin(); i != aSector->Children.end(); i++)
	{
		//add children to node
		pugi::xml_node lChildNode = lNode.append_child(i->first.c_str());
		SaveToNode(i->second,lChildNode);
	}
}

void ProgressTracker::ProgressTrackerImpl::LoadFromNode(Section* aSector, pugi::xml_node& aNode)
{
	pugi::xml_node lValsNode = aNode.child("ValuesNode");

	for (pugi::xml_node iterator = lValsNode.first_child(); iterator; iterator = iterator.next_sibling())
	{
		aSector->Values.insert(std::make_pair(iterator.name(), std::stof(iterator.value())));
	}

	pugi::xml_node lChildrenNode = aNode.child("ChildrenNode");

	for (pugi::xml_node iterator = lChildrenNode.first_child(); iterator; iterator = iterator.next_sibling())
	{
		Section* lSection = new Section();
		LoadFromNode(lSection,iterator);
		lSection->Children.insert(std::make_pair(iterator.name(), lSection));
	}

}

#pragma endregion

#pragma region PUBLIC API

bool ProgressTracker::LoadFile(const char* save_loc)
{
	ProgressTrackerImpl* lImpl = dynamic_cast<ProgressTrackerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	std::stringstream lStr;
	lStr << "Loading variables from: " << save_loc;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.child("save_state");

	if (BaseSaveSection != nullptr)
		delete BaseSaveSection;

	BaseSaveSection = new Section();
	lImpl->LoadFromNode(BaseSaveSection,save_file_node);


	return true;
}

bool ProgressTracker::SaveFile(const char* save_loc)
{
	ProgressTrackerImpl* lImpl = dynamic_cast<ProgressTrackerImpl*>(mPartFuncts);
	if (!lImpl)
	{
		Logger::Console_log(LogLevel::LOG_ERROR, "Wrong format on the implementation class");
		return 0;
	}

	std::stringstream lStr;
	lStr << "Saving variables to: " << save_loc;
	Logger::Console_log(LogLevel::LOG_INFO, lStr.str().c_str());

	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.append_child("save_state");

	if (BaseSaveSection != nullptr)
		delete BaseSaveSection;

	lImpl->SaveToNode(BaseSaveSection, save_file_node);

	config_file.save_file(save_loc);

	return true;
}

bool ProgressTracker::CanLoadGame(const char * file)
{
	pugi::xml_document	file_to_check;
	pugi::xml_parse_result result = file_to_check.load_file(file);

	if (result == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}

}
#pragma endregion

#pragma region Section

Section* Section::AddNewChild(const char* aChildName)
{
	Section* ret = new Section();

	Children.insert(std::make_pair(aChildName, ret));

	return ret;
}

Section* Section::RemoveChild(const char* aChildName)
{
	Section* lPair = Children.find(aChildName)->second;
	delete lPair;
	Children.erase(aChildName);
	return nullptr;
}

Section* Section::GetChild(const char* aChildName)
{
	if (Children.size() > 0 && Children.count(aChildName) > 0)
	{
		return Children.find(aChildName)->second;
	}
	else
	{
		return nullptr;
	}
}

bool Section::SetValue(const char* aValueName, float aValue)
{
	if (Values.count(aValueName) == 0)
	{
		Values.insert(std::make_pair(aValueName, aValue));
	}
	else
	{
		Values.find(aValueName)->second = aValue;
	}
	return true;
}

bool Section::RemoveValue(const char* aValueName)
{
	if (Values.count(aValueName))
	{
		Values.erase(aValueName);
	}
	return true;
}

float Section::GetValue(const char* aValueName)
{
	if (Values.count(aValueName))
	{
		return Values.find(aValueName)->second;
	}

	return 0.0f;
}

#pragma endregion