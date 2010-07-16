#ifndef __STATS_DB_HEADER__
#define __STATS_DB_HEADER__

#include <queue>
#include <map>
#include "EibStdLib.h"
#include "CTime.h"
#include "EibNetwork.h"
#include "DataBuffer.h"
#include "ISerializeable.h"
#include "EIBAddress.h"

#define DEFAULT_MAX_NUM_OBJECTS_HISTORY 100
#define DEFAULT_MAX_NUM_OBJECT_VALUE_HISTORY 10

extern EIB_STD_EXPORT int MAX_NUM_OBJECT_HISTORY;
extern EIB_STD_EXPORT int MAX_NUM_OBJECTS;

class CEIBObjectRecord; 

using namespace EibStack;

class EIB_STD_EXPORT CEIBRecord
{
public:
	CEIBRecord();
	CEIBRecord(const CEIBRecord& rhs);
	virtual ~CEIBRecord();
	
	const CTime& GetTime() const { return _time;}
	const unsigned char* GetValue() const { return _value;}
	unsigned char GetValueLength() const { return _value_len;}
	void SetValueLength(unsigned char len) { _value_len = len;}
	friend class CEIBObjectRecord;

	CString PrintValue() const;

private:
	CTime _time;
	unsigned char _value[MAX_EIB_VALUE_LEN];
	unsigned char _value_len;
};

class EIB_STD_EXPORT CEIBObjectRecord
{
public:
	CEIBObjectRecord();
	virtual ~CEIBObjectRecord();

	void Print(CString& str) const;
	void AddRecord(unsigned char* value, unsigned char value_len);
	
	int GetNumRecords() const { return (int)_history.size();};
	
	const deque<CEIBRecord>& GetHistory() const { return _history;};
	
	const CEibAddress& GetFunction() const{ return _function;}
	void SetFunction(const CEibAddress& function) { _function = function;}

private:
	deque<CEIBRecord> _history;
	CEibAddress _function;
};

class EIB_STD_EXPORT CStatsDB
{
public:
	CStatsDB();
	virtual ~CStatsDB();

	void Init();
	int GetTotalPacketsNum() { return _num_packets_received;}
	void AddRecord(const CEibAddress& function, unsigned char* value, unsigned char value_len);

	bool GetRecord(const CEibAddress& function, CEIBObjectRecord& record) const;

	map<CEibAddress,CEIBObjectRecord>& GetData() { return _db;}

	static int GetMaxSize();
	void Print(CString& str);

private:
	map<CEibAddress,CEIBObjectRecord> _db;
	int _num_packets_received;
};

#endif
