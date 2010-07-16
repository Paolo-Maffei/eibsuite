#ifndef __TIME_HEADER__
#define __TIME_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "CException.h"

#define DEFAULT_TIME_STR_LEN 100

class EIB_STD_EXPORT CTime
{
private:
	int _time_val;
	static struct tm* EibTime(const int* timer);
	static struct tm* EibGMTime(const int* timer);
	static struct tm* EIBTtime_r(const int* timer, struct tm* res);
	static struct tm* EibGMTime_r(const int* timer, struct tm* res);

public:

	CTime();
	CTime(const CTime& time);

	explicit CTime(int time);
	explicit CTime(struct tm *pTime);
	explicit CTime(const char *time, bool local_time = false);
	explicit CTime(const int day, const int month, const int year,const int hour=0, const int minutes=0,const int sec=0);
	virtual ~CTime();

	void Initialize();
	int GetTime() const; //Returns a int that corresponds to this CTime object.
	const int *GetTimePtr() const { return &_time_val;} //Returns a pointer to the attribute _time_val.
	void SetTime(int t) { _time_val = t; }
	int secTo() const; // Return how many seconds is ('current time' - time_val)

	//Converts a CTime object into a formatted string and concat it to the input string
	void AddFormatToString(CString& result,bool get_local) const ;
	CString Format(bool get_local = true) const; //Converts a CTime object into a formatted string.

	void SetNow();
	void SetTimeZero();
	int	GetTimeZero()	const;
	bool IsTimeZero()	const;

	// Supporting return of time string formats
	CString GetTimeStrSegment(CString format, int size = DEFAULT_TIME_STR_LEN, bool get_local = false);
	void AddTimeStrSegmentToString(CString& result, CString format, int size = DEFAULT_TIME_STR_LEN, bool get_local = false);

	CTime& operator=(const CTime&);
	bool operator==(const CTime&) const;
	bool operator!=(const CTime&) const;
	bool operator<(const CTime&) const;
	bool operator>(const CTime&) const;
	bool operator<=(const CTime&) const;
	bool operator>=(const CTime&) const;

	friend EIB_STD_EXPORT CTime  operator-(const CTime& t1, const CTime& t2);
	friend EIB_STD_EXPORT CTime  operator-(const CTime& t1, const int t2);
	CTime& operator-=(const CTime&);
	CTime& operator-=(const int);
	CTime& operator=(const int);

	friend EIB_STD_EXPORT CTime  operator+(const CTime& t1, const CTime& t2);
	friend EIB_STD_EXPORT CTime  operator+(const CTime& t1, const int t2);
	CTime& operator+=(const CTime&);
	CTime& operator+=(const int);

	static bool SetLocalTime(const CTime &time);
	static bool SetLocalDate(const CTime &time);
	static bool GetTimeZone(int &timezone);
	static bool SetTimeZone(const int timezone);
#ifndef WIN32
	static bool SetTimeZoneName(const CString& timezone);
#endif

};

#endif
