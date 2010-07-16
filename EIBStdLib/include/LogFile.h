#ifndef __LOG_FILE_HHEADER__
#define __LOG_FILE_HHEADER__

#include "EibStdLib.h"
#include "JTC.h"
#include "CTime.h"
#include "CString.h"
#include "Utils.h"
#include "Directory.h"

using namespace std;

#define KB 1024
#define LOG_FILE_MAX_SIZE (512 * KB)

#define LOG_LEVEL_INFO_STR "Info: "
#define LOG_LEVEL_ERROR_STR "Error: "
#define LOG_LEVEL_DEBUG_STR "Debug: "

enum EIB_STD_EXPORT LogLevel
{
	LOG_LEVEL_CONSOLE = 0,
	LOG_LEVEL_DEBUG = 1,
	LOG_LEVEL_INFO = 2,
	LOG_LEVEL_ERROR = 3
};

enum EIB_STD_EXPORT TEXT_COLOR
{
	GREEN,
	WHITE,
	RED,
	BLUE,
	YELLOW
};

class EIB_STD_EXPORT CLogFile : public JTCMonitor
{
public:
	CLogFile();
	virtual ~CLogFile();
	void Init(const CString& file_name);

	void Log(LogLevel level, const char* data,...);
	void Log(LogLevel level, const CString& data);
	void SetConsoleColor(TEXT_COLOR color);
	void SetPrompt(bool val) { _print2screen = val;}

	void SetLogLevel(LogLevel level) { _log_level = level;}

private:
	void AppendTimeLine();

private:
	CString _file_name;
	ofstream _file;
	bool _print2screen;
	LogLevel _log_level;
};

#endif
