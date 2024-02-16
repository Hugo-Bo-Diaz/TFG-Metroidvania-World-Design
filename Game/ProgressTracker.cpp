#include "ProgressTracker.h"
#include "Application.h"
#include "SceneController.h"
#include "Physics.h"
#include "Gui.h"
#include "Camera.h"

#include <string>

ProgressTracker::ProgressTracker()
{
	name = "ProgressTracker";

}

bool ProgressTracker::Init()
{
	BaseSaveSection = new Section();
	BaseSettingsSection = new Section();

	return true;
}

bool ProgressTracker::LoadConfig(pugi::xml_node & node)
{
	//pugi::xml_node lore_location = node.child("lore_file");

	//LoadLogs(lore_location.attribute("path").as_string());

	return true;
}

bool ProgressTracker::CreateConfig(pugi::xml_node & node)
{
	//pugi::xml_node lore_location = node.append_child("lore_file");

	//lore_location.append_attribute("path") = "lore.xml";
	//LoadLogs("lore.xml");

	return true;
}

bool ProgressTracker::LoadFile(const char* save_loc)
{
	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.child("save_state");

	BaseSaveSection->LoadFromNode(save_file_node);


	return true;
}

bool ProgressTracker::SaveFile(const char* save_loc)
{
	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.append_child("save_state");

	BaseSaveSection->SaveToNode(save_file_node);

	config_file.save_file(save_loc);

	return true;
}

bool ProgressTracker::Loop(float dt)
{

	return true;
}

bool ProgressTracker::CleanUp()
{
	return true;
}

void ProgressTracker::SaveGame(const char * file)
{
	bool create_file = false;
	pugi::xml_document	config_file;
	pugi::xml_node save_file_node;

	save_file_node = config_file.append_child("save_state");

	

	config_file.save_file(file);
	
}

void ProgressTracker::LoadGame(const char * file)
{
	if (!CanLoadGame(file))
		return;
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
	if (Children.count(aChildName) > 0)
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

void Section::SaveToNode(pugi::xml_node& aNode)
{
	pugi::xml_node lValsNode = aNode.append_child("ValuesNode");

	for (auto i = Values.begin(); i != Values.end(); i++)
	{
		//add values to node
		pugi::xml_node lNode = lValsNode.append_child(i->first.c_str());
		lNode.text().set(std::to_string(i->second).c_str());
	}

	pugi::xml_node lNode = aNode.append_child("ChildrenNode");

	for (auto i = Children.begin(); i != Children.end(); i++)
	{
		//add children to node
		pugi::xml_node lChildNode = lNode.append_child(i->first.c_str());
		i->second->SaveToNode(lChildNode);
	}
}

void Section::LoadFromNode(pugi::xml_node& aNode)
{
	pugi::xml_node lValsNode = aNode.child("ValuesNode");

	for (pugi::xml_node iterator = lValsNode.first_child(); iterator; iterator = iterator.next_sibling())
	{
		Values.insert(std::make_pair(iterator.name(), std::stof( iterator.value())));
	}

	pugi::xml_node lChildrenNode = aNode.child("ChildrenNode");

	for (pugi::xml_node iterator = lChildrenNode.first_child(); iterator; iterator = iterator.next_sibling())
	{
		Section* lSection = new Section();
		lSection->LoadFromNode(iterator);
		Children.insert(std::make_pair(iterator.name(),lSection));
	}
}
#pragma endregion