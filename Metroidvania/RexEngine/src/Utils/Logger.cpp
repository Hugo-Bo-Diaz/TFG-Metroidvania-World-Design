#include "Utils/Logger.h"
#include <ctime>
#include <thread>
#include <sstream>
#include <iostream>
#include <fstream>

Logger::Logger()
{

}

bool Logger::Init()
{
	return true;
}

void Logger::Console_log(LogLevel aLevel, const char* log)
{
	Log l;

	l.Text = log;
	l.level = aLevel;
	l.date = std::time(0);

	std::string lLogStr = l.GetAsString();
	printf(lLogStr.c_str());

	std::fstream file;
		file.open(	"logs.txt", std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::app);

	if (!file)
	{
		file.open("logs.txt", std::fstream::binary | std::fstream::trunc | std::fstream::out | std::fstream::app);
		file.close();
		// re-open with original flags
		file.open("logs.txt", std::fstream::binary | std::fstream::in | std::fstream::out| std::fstream::app);
	}

	file.write(lLogStr.c_str(),lLogStr.size());
}


std::string Log::GetAsString()
{
	std::string lResult = "";
	switch (level)
	{
	case LogLevel::LOG_DEBUG:
		lResult += "DEBG";
		break;
	case LogLevel::LOG_ERROR:
		lResult += "ERRO";
		break;
	case LogLevel::LOG_INFO:
		lResult += "INFO";
		break;
	case LogLevel::LOG_WARN:
		lResult += "WARN";
		break;
	}

	lResult += " - ";

	char str[26];
	//ctime_s(str,sizeof(str), &date);

	
	struct tm timeinfo;
	localtime_s(&timeinfo,&date);

	strftime(str,sizeof(str), "%d-%m-%Y %H:%M:%S",&timeinfo);
	lResult += str;

	lResult += " - ";
	
	auto myid = std::this_thread::get_id();
	std::stringstream ss;
	ss << myid;


	lResult += ss.str();

	lResult += " - ";

	lResult += Text;
	lResult += "\n";
	return lResult;
}
