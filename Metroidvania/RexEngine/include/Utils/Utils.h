#ifndef UTILS__H
#define UTILS__H

#include <list>
#include <string>
#include <Windows.h>
#include <typeindex>
#include <queue>

bool FileExists(const std::string& name);

std::string GetDirectoryFromPath(std::string aPath);

void GetAllExtensionPathRecursive(const char* path, const char* extension, std::list<std::string>& listToFill);

template<class T>
static inline std::type_index GetTypeIndex()
{
	return std::type_index(typeid(T));
}

float GetQueueMedianNumber(std::queue<float> lQueue);


static inline void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

static inline void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

static inline bool IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}
#endif