#include "CTime.h"

void CTime::Initialize()
{
#ifdef WIN32
	_tzset(); // set time zone
#else
	tzset(); // set the time zone
#endif
}

/**
	CTime Constructors
**/
CTime::CTime()
{
	_time_val = static_cast<int>(time(0));
}

CTime::CTime(int time)
{
	_time_val = time;
}

//CTime::CTime(int time)
//{
//	_time_val = time;
//	GAssert(_time_val != (int)(-1));
//}

CTime::CTime(const CTime& time)
{
	_time_val = time.GetTime();
}

CTime::CTime(struct tm *pTime)
{
	_time_val = ((pTime!=NULL) ? static_cast<int>(mktime(pTime)): static_cast<int>(time(0)));
}

CTime::CTime(const int day, const int month, const int year,const int hour, const int minutes,const int sec)
{
	struct tm intm;

	// NOTE: struct tm months are 0 to 11

	intm.tm_hour = (int)(hour);
	intm.tm_min = (int)(minutes);
	intm.tm_sec = (int)(sec);
	intm.tm_mday = (int)(day);
	intm.tm_mon = (int)(month) - 1;
	intm.tm_year = (int)(year) - 1900;
	intm.tm_isdst = -1; // day light saving time effect

	_time_val = static_cast<int>(mktime(&intm));
}


// Converts a unix style time string
// of the format: Tue May 03 21:51:03 1994
CTime::CTime(const char *time, bool local_time)
{
	int hour, min, sec, day, month, year;
	char month_s[10], trash;
	char day_s[10], hour_s[10], min_s[10];
	char year_s[10];

	if (sscanf(time, "%9s %9s %4s %2s:%2s %c",day_s, month_s, year_s,hour_s, min_s, &trash) != 5) {
		throw CEIBException(SystemError,"Cannot initialize time class");
	}

	day = atoi(day_s);
	hour = atoi(hour_s);
	min = atoi(min_s);
	sec = 0;
	year = atoi(year_s);
	// NOTE: struct tm months are 0 to 11
	if (!strcmp(month_s,"Jan")) month = 0 ;
	else if (!strcmp(month_s,"Feb")) month = 1 ;
	else if (!strcmp(month_s,"Mar")) month = 2 ;
	else if (!strcmp(month_s,"Apr")) month = 3 ;
	else if (!strcmp(month_s,"May")) month = 4 ;
	else if (!strcmp(month_s,"Jun")) month = 5 ;
	else if (!strcmp(month_s,"Jul")) month = 6 ;
	else if (!strcmp(month_s,"Aug")) month = 7 ;
	else if (!strcmp(month_s,"Sep")) month = 8 ;
	else if (!strcmp(month_s,"Oct")) month = 9 ;
	else if (!strcmp(month_s,"Nov")) month = 10;
    else if (!strcmp(month_s,"Dec")) month = 11;
	else {
		throw CEIBException(SystemError,"Cannot initialize time class");
		month = 12; //eliminate compiler warning on "may be used uninitialized".
	}

	struct tm intm;

	intm.tm_hour = (int)(hour);
	intm.tm_min = (int)(min);
	intm.tm_sec = (int)(sec);
	intm.tm_mday = (int)(day);
	intm.tm_mon = (int)(month);
	intm.tm_year = ((int)(year) - 1900);
	if(local_time) {
		// When converting a string we assume it was procuced by gmtime(). If we want
		// local time we ask the Operating System to take into account the daylight
		// savings (-1) as int as the time differences. If we want gmt time we do not
		// want daylight savings effect (0). In this case we need to cancel the effect
		// of the timezone diffrences later on (See the last if in this method)
		intm.tm_isdst = -1;
	}else{
		intm.tm_isdst = 0;
	}

	_time_val = static_cast<int>(mktime(&intm));


	if (!local_time) {
		// By default local_time is set to false, when converting a string we
		// assume it was produced by gmtime()
		// which means that time in the string is according to "UTC time zone"
		// To convert the time on the local machine we set tm_isdst to -1 and
		// use _timezone to adjust local time.
#ifdef WIN32
		_time_val -= _timezone;
#else
		_time_val -= timezone;
#endif
	}
}


CTime::~CTime() {};
/**
@return int that corresponds to this CTime object.
**/
int CTime::GetTime() const
{
	return _time_val;
}

/**
Calculate seconds to the given time.
@return how many seconds is ('current time' - time_val)
**/
int CTime::secTo() const
{
	int sec;

	sec = GetTime() - static_cast<int>(time(0));
	sec = ((sec < 0) ? 0 : sec);

	return sec;
}

/**
Converts a CTime object into a formatted string (unix style)
based on the local time zone.
**/
CString CTime::Format(bool get_local) const
{
	CString result ;
	AddFormatToString ( result , get_local ) ;
	return result ;
}
void CTime::AddFormatToString(CString& result,bool get_local) const
{
	char strDest[25];
	size_t maxsize = 25;
	int tt = GetTime();
	struct tm *tm_struct;
	// Using thread safe functions
	if (!get_local) {
		// Use gmtime for UTC format
#ifdef WIN32
		tm_struct = EibGMTime(&tt);
#else
		tm_struct = new struct tm();
		if (!tm_struct) {
			return ;
		}
		EibGMTime_r(&tt, tm_struct);
#endif
	} else {
		// Use localtime for local time format
#ifdef WIN32
		tm_struct = EibTime(&tt);
#else
		tm_struct = new struct tm();
		if (!tm_struct) {
			return ;
		}
		EIBTtime_r(&tt, tm_struct);
#endif
	}

	// We keep the format of ctime, for example "Mon Aug 21 20:07:29 2000"
	(void) strftime(strDest, maxsize, "%a %b %d %H:%M:%S %Y", tm_struct);

#ifndef WIN32
	delete tm_struct;
#endif

	// Supporting UNIX time format.
	if (strDest[8] == ' ') {
		strDest[8] = '0';
	}
	result += strDest ;
}

//Sets the time value to be 'now'
void CTime::SetNow()
{
	_time_val = static_cast<int>(time(0));
}

void CTime::SetTimeZero()
{
	SetTime(GetTimeZero());
}


int CTime::GetTimeZero() const
{
	CTime tmp(STRING_TIME_ZERO.GetBuffer());
	return tmp.GetTime();
}

bool CTime::IsTimeZero() const
{
	return ( GetTimeZero() == GetTime() );
}


CString CTime::GetTimeStrSegment(CString format, int size, bool get_local)
{
	CString res;

	AddTimeStrSegmentToString(res, format, size, get_local);

	return res;
}

void CTime::AddTimeStrSegmentToString(CString& result, CString format, int size , bool get_local )
{
	char* strDest = new char[size];
	if (!strDest) {
		result += EMPTY_STRING;
		return ;
	}
	size_t maxsize = (unsigned int)(size);

	int tt = GetTime();
	struct tm *tm_struct;
	// Using thread safe functions
	if (!get_local) {
		// Use gmtime for UTC format
#ifdef WIN32
		tm_struct = EibGMTime(&tt);
#else
		tm_struct = new struct tm();
		if (!tm_struct) {
			result += EMPTY_STRING;
			delete []strDest;
			return ;
		}
		EibGMTime_r (&tt, tm_struct);
#endif
	} else {
		// Use localtime for local time format
#ifdef WIN32
		tm_struct = EibTime(&tt);
#else
		tm_struct = new struct tm();
		if (!tm_struct) {
			result += EMPTY_STRING;
			delete []strDest;
			return ;
		}
		EIBTtime_r(&tt, tm_struct);
#endif
	}

	(void) strftime(strDest, maxsize, format.GetBuffer(), tm_struct);

#ifndef WIN32
	delete tm_struct;
#endif

	result += strDest;
	delete []strDest;
}

/**
  relation operators
**/

CTime& CTime::operator=(const CTime& t)
{
	_time_val = t.GetTime();
	return *this;
}

bool CTime::operator==(const CTime& time2) const
{
	return (GetTime() == time2.GetTime());
}
bool CTime::operator!=(const CTime& time2) const
{
	return (GetTime() != time2.GetTime());
}
bool CTime::operator<(const CTime& time2) const
{
	return (GetTime() < time2.GetTime());
}
bool CTime::operator>(const CTime& time2) const
{
	return (GetTime() > time2.GetTime());
}
bool CTime::operator>=(const CTime& time2) const
{
	return (GetTime() >= time2.GetTime());
}
bool CTime::operator<=(const CTime& time2) const
{
	return (GetTime() <= time2.GetTime());
}


CTime& CTime::operator-=(const CTime& t2)
{
	this->_time_val -= t2.GetTime();
	return *this;
}

CTime& CTime::operator-=(const int t2)
{
	this->_time_val -= t2;
	return *this;
}

CTime& CTime::operator=(const int t2)
{
	this->_time_val = t2;
	return *this;
}

EIB_STD_EXPORT CTime operator-(const CTime& t1, const CTime& t2)
{

	CTime res(t1.GetTime() - t2.GetTime());
	return res;
}

EIB_STD_EXPORT CTime operator-(const CTime& t1, const int t2)
{

	CTime res(t1.GetTime() - t2);
	return res;
}

CTime& CTime::operator+=(const CTime& t2)
{
	this->_time_val += t2.GetTime();
	return *this;
}

CTime& CTime::operator+=(const int t2)
{
	this->_time_val += t2;
	return *this;
}


EIB_STD_EXPORT CTime operator+(const CTime& t1, const CTime& t2)
{

	CTime res(t1.GetTime() + t2.GetTime());
	return res;
}

EIB_STD_EXPORT CTime operator+(const CTime& t1, const int t2)
{

	CTime res(t1.GetTime() + t2);
	return res;
}

bool CTime::SetLocalTime(const CTime &time_to_set)
{
#ifdef WIN32
	int tmp_time = time_to_set.GetTime();
	struct tm *tmp_tm = CTime::EibTime(&tmp_time);
	if (!tmp_tm)
		return false;

	SYSTEMTIME systemTime;
	systemTime.wDay = tmp_tm->tm_mday;
	systemTime.wMonth = tmp_tm->tm_mon + 1;
	systemTime.wYear = tmp_tm->tm_year + 1900;
	systemTime.wHour = tmp_tm->tm_hour;
	systemTime.wMinute = tmp_tm->tm_min;
	systemTime.wSecond = tmp_tm->tm_sec;
	systemTime.wMilliseconds = 0;
	if (!::SetLocalTime(&systemTime))
		return false;
	return true;
#else
	struct timeval tv = {  (int)(time_to_set.GetTime()), 0 };
	if (settimeofday(&tv, NULL) < 0) {
		perror("settimeofday");
		return false;
	}

	return true;
#endif
}


bool CTime::SetLocalDate(const CTime &into_set)
{
#ifdef WIN32

	SYSTEMTIME systemTime;
	int tmp_time = into_set.GetTime();
	struct tm *tmp_tm = EibTime(&tmp_time);
	if (!tmp_tm)
		return false;
	systemTime.wDay = tmp_tm->tm_mday;
	systemTime.wMonth = tmp_tm->tm_mon + 1;
	systemTime.wYear = tmp_tm->tm_year + 1900;
	int now_t = static_cast<int>(time(0));
	tmp_tm = EibTime(&now_t);
	if (!tmp_tm)
		return false;
	systemTime.wHour = tmp_tm->tm_hour;
	systemTime.wMinute = tmp_tm->tm_min;
	systemTime.wSecond = tmp_tm->tm_sec;
	systemTime.wMilliseconds = 0;
	if (!::SetLocalTime(&systemTime))
		return false;
	return true;
#else
	int tmp_time = into_set.GetTime();
	int cur_time = time(NULL);
	struct tm tmp_tm;
	struct tm cur_tm;
	EIBTtime_r(&tmp_time, &tmp_tm);
	EIBTtime_r(&cur_time, &cur_tm);

	tmp_tm.tm_sec = cur_tm.tm_sec;
	tmp_tm.tm_min = cur_tm.tm_min;
	tmp_tm.tm_hour = cur_tm.tm_hour;

	struct timeval tv = {  mktime(&tmp_tm), 0 };
	if (settimeofday(&tv, NULL) < 0) {
		perror("settimeofday");
		return false;
	}

	return true;
#endif
}

/**
 Get the current time zone
 @param int &timezone Time zone value to get in minutes. Verify: localtime = UTC + timezone
 @return bool true if successfull, false if errors (see CGErrorMgr::GetLastWinMsg())
 **/
bool CTime::GetTimeZone(int &answer)
{
#ifdef WIN32
	TIME_ZONE_INFORMATION tzi;
	if (GetTimeZoneInformation(&tzi) == TIME_ZONE_ID_INVALID)
		return false;
	answer = -tzi.Bias;
	return true;
#else
	tzset();
	answer = -(timezone / 60);
	return true;
#endif
}

/**
 Set the current time zone
 @param const int timezone Time zone value to set in minutes. Verify: localtime = UTC + timezone
 @return bool true if successfull, false if errors (see CGErrorMgr::GetLastWinMsg())
 **/
bool CTime::SetTimeZone(const int timezone)
{
#ifdef WIN32
	TIME_ZONE_INFORMATION tzi;
	if (GetTimeZoneInformation(&tzi) == TIME_ZONE_ID_INVALID)
		return false;
	tzi.Bias = -(long)(timezone);
	if (!SetTimeZoneInformation(&tzi))
		return false;
	return true;
#else
	if (system("tzselect > /var/tmp/tz_out.txt") != 0) {
		return false;
	}
	FILE *f;
	CString zonename;
	if ((f = fopen("/var/tmp/tz_out.txt", "r"))) {
		char buffer[128];
		//unsigned long len, cnt = 0;
		if ( fscanf(f, "%s", buffer) != 1)
		{
			fclose (f);
			return false;
		}
		zonename = buffer;
		fclose (f);
	} else {
		return false;
	}

	return SetTimeZoneName(zonename);
#endif
}


#ifndef WIN32
bool CTime::SetTimeZoneName(const CString& timezone)
{
	/*
	static const CString sysconfig_fname("/etc/sysconfig/clock");
	static const CString localtime_fname("/etc/localtime");
	static const CString zoneinfo_dname("/usr/share/zoneinfo");
	const CString zone_fname = zoneinfo_dname + "/" + timezone;
	struct stat stat_st;

	if (!CopyFile(zone_fname, localtime_fname)) {
		return false;
	}

	if (stat(sysconfig_fname.GetBuffer(), &stat_st) == 0) {
		FILE *f;
		if ((f = fopen(sysconfig_fname.GetBuffer(), "w"))) {
			fprintf(f, "ZONE=%s\nUTC=false\nARC=false\n", timezone.GetBuffer());
			fclose(f);
		} else {
			return false;
		}
	}
	*/
	return true;
}
#endif

struct tm* CTime::EibTime(const int* timer)
{
	time_t l_timer; //temporary variable to be used as argument to the original time method;
	struct tm* ret_val;

	if(timer)
	{
		l_timer = (int)(*timer);
		ret_val = localtime(&l_timer);
	}
	else
	{
		ret_val = gmtime(NULL);
	}

	return ret_val;
}

struct tm* CTime::EibGMTime(const int* timer)
{
	time_t l_timer; //temporary variable to be used as argument to the original time method;
	struct tm* ret_val;

	if(timer)
	{
		l_timer = (int)(*timer);
		ret_val = gmtime(&l_timer);
	}
	else
	{
		ret_val = gmtime(NULL);
	}

	return ret_val;
}

#ifndef WIN32

struct tm* CTime::EIBTtime_r(const int* timer, struct tm* res)
{
	time_t l_timer;
	struct tm* ret_val;

	if (timer)
	{
		l_timer = (int)(*timer);
		ret_val = gmtime_r(&l_timer, res);
	}
	else
	{
		ret_val = gmtime_r(NULL, res);
	}

	return ret_val;
}

struct tm* CTime::EibGMTime_r(const int* timer, struct tm* res)
{
	time_t l_timer;
	struct tm* ret_val;

	if (timer)
	{
		l_timer = (int)(*timer);
		ret_val = localtime_r(&l_timer, res);
	}
	else
	{
		ret_val = localtime_r(NULL, res);
	}

	return ret_val;
}

#endif
