#ifndef PROGRESS__TRACKER__H
#define PROGRESS__TRACKER__H

#include "PartsDef.h"
#include <vector>
#include <map>
#include "Part.h"

#define MAX_EXPANSIONS 32

class DLL_EXPORT Section
{
public:
	std::map<std::string, Section*> Children;
	std::map<std::string, float>Values;

	Section* AddNewChild(const char* aChildName);
	Section* RemoveChild(const char* aChildName);
	Section* GetChild(const char* aChildName);

	bool SetValue(const char* aValueName,float aValue);
	bool RemoveValue(const char* aValueName);
	float GetValue(const char* aValueName);
	std::vector<float> GetSectionValues() {
		std::vector<float> valueList;
		for (std::map<std::string,float>::const_iterator it = Values.begin(); it != Values.end(); ++it) {
			valueList.push_back(it->second);
		}
		return valueList;
	}
};

//module that can be used to easily store data
class DLL_EXPORT ProgressTracker : public Part
{
public:
	ProgressTracker(EngineAPI& aAPI);

	//returns a section that can be used for a save file
	Section* GetBaseSaveSection();
	//returns a section that can be used for settings
	Section* GetBaseSettingsSection();

	//returns true if file can be loaded
	bool CanLoadGame(const char* file);
	//returns true if file was loaded
	bool LoadFile(const char* save_loc);
	//returns true if file was saved
	bool SaveFile(const char* save_loc);
private:
	class ProgressTrackerImpl;
};

#endif // !CAMERA__H
