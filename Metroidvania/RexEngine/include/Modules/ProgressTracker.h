#ifndef PROGRESS__TRACKER__H
#define PROGRESS__TRACKER__H

#include "PartsDef.h"
#include <vector>
#include <map>
#include "Part.h"

#define MAX_EXPANSIONS 32

struct DLL_EXPORT LoreLog
{
	std::string title;
	std::string text;
	int id;

	bool operator<(const LoreLog& rhs) const
	{
		if (id < rhs.id)
			return true;
		else
			return false;
	}
};


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

class DLL_EXPORT ProgressTracker : public Part
{
public:
	ProgressTracker(EngineAPI& aAPI);

	Section* BaseSaveSection;
	Section* BaseSettingsSection;

	bool CanLoadGame(const char* file);
	bool LoadFile(const char* save_loc);
	bool SaveFile(const char* save_loc);
private:
	class ProgressTrackerImpl;
};

#endif // !CAMERA__H
