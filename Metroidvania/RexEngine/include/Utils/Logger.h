#ifndef LOGGER__H
#define LOGGER__H

#include <string>
#include <chrono>

#define MAX_TIME_LOG 2 // seconds
#define DLL_EXPORT __declspec(dllexport)

enum LogLevel{
LOG_ERROR,
LOG_WARN,
LOG_INFO,
LOG_DEBUG
};

struct Log{
	time_t date; // 2
	int threadID; // 3

	LogLevel level; // 1
	std::string Text; // 4

	std::string GetAsString();
};

class DLL_EXPORT Logger
{
public:

	Logger();

	bool Init();
	
	static void Console_log(LogLevel level, const char* log);
};

#endif //CONSOLE__H