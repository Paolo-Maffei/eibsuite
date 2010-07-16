#ifndef __WEB_COLLECTOR_HEADER__
#define __WEB_COLLECTOR_HEADER__

#include "JTC.h"
#include "StatsDB.h"
#include "CTime.h"

typedef struct ScheduledCommand
{
	ScheduledCommand(const CTime& time, const CEibAddress& dst, unsigned char* value, unsigned char val_len)
	{
		_time = time;
		_dst = dst;
		_value_len = val_len;
		memcpy(_value,value,val_len);
	};

	CTime _time;
	CEibAddress _dst;
	unsigned char _value_len;
	unsigned char _value[MAX_EIB_VALUE_LEN];
}ScheduledCommand;

class ScheduledCommandComparison
{
public:
	ScheduledCommandComparison() {};
	virtual ~ScheduledCommandComparison() {};
  
	bool operator() (const ScheduledCommand& lhs, const ScheduledCommand& rhs) const
	{
		return (lhs._time > rhs._time);
	}
};

typedef priority_queue<ScheduledCommand,vector<ScheduledCommand>,ScheduledCommandComparison > SchedQueue;

class CWebCollector : public JTCThread, public JTCMonitor
{
public:
	CWebCollector();
	virtual ~CWebCollector();

	virtual void run();
	void Close();
	CStatsDB GetStatsDB();

	void CheckForScheduledCommand();
	bool AddScheduledCommand(const CTime& time,
							 const CEibAddress& dst,
							 unsigned char* value,
							 unsigned char val_len,
							 CString& err_str);

	SchedQueue GetQueue() { return _schedule; }

	void ScheduleToHtml(CDataBuffer& content);

private:
	CStatsDB _db;
	bool _stop;
	SchedQueue _schedule;
};



#endif
