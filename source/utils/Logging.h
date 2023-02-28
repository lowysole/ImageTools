#pragma once

#include "Globals.h"

#include <string>
#include <mutex>
#include <queue>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(format, ...) logger->Log(__FILENAME__, __LINE__, format, __VA_ARGS__);

#define FPS_LOG_SIZE 100

class Logger {
public:
	void Log(const char file[], int line, const char* format, ...);
	void UpdateLogString();

public:
	std::string logString = "";

private:
	std::mutex logMessageQueueMutex;
	std::queue<std::string> logMessageQueue;

	char tmpString[4096] = {0};
	char tmpString2[4096] = {0};
};

extern Logger* logger;