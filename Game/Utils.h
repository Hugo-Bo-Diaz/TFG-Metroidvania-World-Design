#ifndef UTILS__H
#define UTILS__H

#include <list>
#include <string>
#include <Windows.h>


static inline bool FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

static std::string GetDirectoryFromPath(std::string aPath)
{
	std::string directory;
	size_t last_slash_idx = aPath.rfind('\\');
	if (std::string::npos == last_slash_idx)
	{
		last_slash_idx = aPath.rfind('/');
	}
	if (std::string::npos != last_slash_idx)
	{
		std::string lDir = aPath.substr(0, last_slash_idx) + "/";
		return lDir;
	}
	return "";
}

static void GetAllExtensionPathRecursive(const char* path, const char* extension, std::list<std::string>& listToFill)
{
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);

	WIN32_FIND_DATAA fdFile;
	HANDLE hFind = NULL;

	std::wstring conver((const wchar_t*)&pwd[0], sizeof(pwd) / sizeof(pwd[0])); //convert to wstring
	std::string fulldir(conver.begin(), conver.end());

	fulldir += path;
	fulldir += "\\*";

	if ((hFind = FindFirstFileA(fulldir.c_str(), &fdFile)) == INVALID_HANDLE_VALUE)
	{
		return;
	}
	fulldir = fulldir.erase(fulldir.size() - 2);

	do
	{
		//first two files
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			//is it a folder
			if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string folder_rel = path;
				folder_rel += "\\";
				folder_rel += fdFile.cFileName;

				GetAllExtensionPathRecursive(folder_rel.c_str(), extension, listToFill);
			}
			else
			{
				std::string s = fdFile.cFileName;

				if (s.substr(s.find_last_of(".") + 1) == extension)
				{
					listToFill.push_back(s);
				}
			}
		}

	} while (FindNextFileA(hFind, &fdFile));

	FindClose(hFind);

}



#endif