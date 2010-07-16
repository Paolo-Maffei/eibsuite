#include "Globals.h"

const CString EMPTY_STRING = "";
const CString STRING_TIME_ZERO= "Thu Jan 02 00:00:00 1970";

#ifdef WIN32
const CString PATH_DELIM = "\\";
#else
const CString PATH_DELIM = "/";
#endif

//Defalut folders section
const CString CURRENT_LOGS_FOLDER = CDirectory::CurrentDirectory() + PATH_DELIM + DEFAULT_LOG_FOLDER + PATH_DELIM;
const CString CURRENT_CONF_FOLDER = CDirectory::CurrentDirectory() + PATH_DELIM + DEFAULT_CONF_FOLDER + PATH_DELIM;
const CString CURRENT_WORKING_FOLDER = CDirectory::CurrentDirectory() + PATH_DELIM;

//Auto discovery service
const CString AUTO_DISCOVERY_SERVICE_ADDRESS = "224.0.23.11";
