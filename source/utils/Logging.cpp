#include "Logging.h"

#include <windows.h>

#include "Leaks.h"

void Logger::Log(const char file[], int line, const char* format, ...) {
	// Construct the string from variable arguments

	logMessageQueueMutex.lock();
	va_list ap;
	va_start(ap, format);
	vsprintf_s(tmpString, 4096, format, ap);
	va_end(ap);

#if _DEBUG
	sprintf_s(tmpString2, 4096, "%s(%d) : %s\n", file, line, tmpString);
#else
	sprintf_s(tmpString2, 4096, "%s\n", tmpString);
#endif
	OutputDebugString(tmpString2);
	logMessageQueue.push(tmpString2);
	logMessageQueueMutex.unlock();
}

void Logger::UpdateLogString() {
	logMessageQueueMutex.lock();
	while (!logMessageQueue.empty()) {
		logString += logMessageQueue.front();
		logMessageQueue.pop();
	}
	logMessageQueueMutex.unlock();
}

Logger* logger = nullptr;
