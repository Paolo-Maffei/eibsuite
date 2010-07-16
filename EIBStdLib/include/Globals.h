#ifndef __GLOBALS_HEADER__
#define __GLOBALS_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "CException.h"
#include "Directory.h"

extern const EIB_STD_EXPORT CString EMPTY_STRING;
extern const EIB_STD_EXPORT CString PATH_DELIM;
extern const EIB_STD_EXPORT CString STRING_TIME_ZERO; // = ("Thu Jan 02 00:00:00 1970");

extern const EIB_STD_EXPORT CString AUTO_DISCOVERY_SERVICE_ADDRESS;
#define AUTO_DISCOVERY_SERVICE_PORT 5454

#define DEFAULT_CONF_FOLDER	"conf"
#define DEFAULT_LOG_FOLDER	"logs"

extern const EIB_STD_EXPORT CString CURRENT_LOGS_FOLDER;
extern const EIB_STD_EXPORT CString CURRENT_CONF_FOLDER;
extern const EIB_STD_EXPORT CString CURRENT_WORKING_FOLDER;

#define TAB "\t"
#ifdef WIN32
#define LINE_BREAK "\r\n"
#else
#define LINE_BREAK "\n"
#define _getcwd getcwd
#endif

#endif
