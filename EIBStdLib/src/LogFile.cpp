#include "LogFile.h"
#include "ConsoleColor.h"

CLogFile::CLogFile():_print2screen(true),_log_level(LOG_LEVEL_INFO)
{
}

CLogFile::~CLogFile()
{
}

void CLogFile::SetConsoleColor(TEXT_COLOR color)
{
#ifdef WIN32
	switch (color)
	{
	case WHITE: cout << white;
		break;
	case BLUE: cout << blue;
		break;
	case RED: cout << red;
		break;
	case GREEN: cout << green;
		break;
	case YELLOW: cout << yellow;
		break;
	}
#else
	switch (color)
	{
		case WHITE: cout << "\033[00;0m";
			break;
		case BLUE: cout << "\033[22;34m";
			break;
		case RED: cout << "\033[22;31m";
			break;
		case GREEN: cout << "\033[22;32m";
			break;
		case YELLOW: cout << "\033[01;33m";
			break;
	}
#endif
}

void CLogFile::Init(const CString& file_name)
{
//YGYG: for now, avoid logging to files
#if 0
	_file_name = file_name;

	int size = CUtils::GetFileSize(file_name);
	if (size > LOG_FILE_MAX_SIZE){
		int result;
		CString new_file_name;
		CUtils::GetTimeStrForFile(new_file_name);
		new_file_name += ".log";
		result = rename( file_name.GetBuffer(),new_file_name.GetBuffer());
		if(result != 0){
			throw CEIBException(FileError,"Error rename of log file");
		}
	}

	if(!CDirectory::Create(CURRENT_LOGS_FOLDER))
	{
		if(!CDirectory::Change(CURRENT_LOGS_FOLDER))
		{
			throw CEIBException(FileError,"Cannot Initialized Logs folder.");
		}
		else{
			CDirectory::Change(CURRENT_WORKING_FOLDER);
		}
	}

	_file.open(_file_name.GetBuffer(),ios::out|ios::app);
	if(_file.fail())
	{
		throw CEIBException(FileError,"Cannot Initialized Log file.");
	}

	_file.close();
#endif
}

void CLogFile::AppendTimeLine()
{
	_file << '[';
	CTime t;
	_file << t.Format().GetBuffer();
	_file << ']';
}

void CLogFile::Log(LogLevel level, const char* format,...)
{
	//only one thread can write the the same log at the same time ... (prevent jibrish)
	JTCSynchronized sync(*this);

	if (level == LOG_LEVEL_ERROR)
	{
		this->SetConsoleColor(RED);
	}

	bool print2file = true;
	if(_log_level > level){
		print2file = false;
	}

//YGYG: for now, avoid logging to files
#if 0
	if(print2file){

		_file.open(_file_name.GetBuffer(),ios::out|ios::app);
		if(_file.fail()){
			throw CEIBException(FileError,"Cannot Open Log file.");
			_file.close();
			return;
		}

		AppendTimeLine();
		
		_file << ' ';

		switch (level)
		{
		case LOG_LEVEL_INFO:
			_file << LOG_LEVEL_INFO_STR;
			break;
		case LOG_LEVEL_ERROR:
			_file << LOG_LEVEL_ERROR_STR;
			break;
		case LOG_LEVEL_DEBUG:
			_file << LOG_LEVEL_DEBUG_STR;
			break;
		default:
			_file << LOG_LEVEL_INFO_STR;
			break;
		}
		
		_file << ' ';
	}
#endif
	CString ret;
	if (NULL != format)
	{
		char status[512];
		int len = 0;

		va_list arglist;
	    va_start(arglist,format);
		len = vsprintf(status,format,arglist);
		va_end(arglist);
		if(print2file){
			//print to the file
			_file << status << endl;
		}
		//print to screen
		if(_print2screen){
			cout.flush();
			cout << status << endl;
			SetConsoleColor(WHITE);
		}
	}
//YGYG: for now, avoid logging to files
#if 0
	if(print2file)
	{
		_file.close();
	}
#endif
}

