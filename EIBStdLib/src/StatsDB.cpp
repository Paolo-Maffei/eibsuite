#include "StatsDB.h"
#include "Utils.h"

int MAX_NUM_OBJECT_HISTORY = DEFAULT_MAX_NUM_OBJECT_VALUE_HISTORY;
int MAX_NUM_OBJECTS = DEFAULT_MAX_NUM_OBJECTS_HISTORY;

CStatsDB::CStatsDB() : _num_packets_received(0)
{
}

CStatsDB::~CStatsDB()
{
}

void CStatsDB::Init()
{
}

void CStatsDB::AddRecord(const CEibAddress& function, unsigned char* value, unsigned char value_len)
{	
	map<CEibAddress,CEIBObjectRecord>::iterator it;
	it = _db.find(function);
	if (it == _db.end()){
		if((int)_db.size() == MAX_NUM_OBJECTS){
			_db.erase(_db.begin());
		}
		CEIBObjectRecord rec;
		rec.SetFunction(function);
		rec.AddRecord(value, value_len);
		_db.insert(pair<CEibAddress,CEIBObjectRecord>(function,rec));
	}else{
		it->second.AddRecord(value, value_len);
	}

	++_num_packets_received;
	
}

bool CStatsDB::GetRecord(const CEibAddress& function, CEIBObjectRecord& record) const
{
	map<CEibAddress,CEIBObjectRecord>::const_iterator it;
	it = _db.find(function);
	if (it != _db.end()){
		record = it->second;
		return true;
	}
	return false;
}

void CStatsDB::Print(CString& str)
{
	map<CEibAddress,CEIBObjectRecord>::iterator it;
	for(it = _db.begin(); it != _db.end(); ++it)
	{
		str += "Function: ";
		str += it->first.ToString();
		str += "\r\n";
		it->second.Print(str);
		str += "*************************\r\n";
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEIBObjectRecord::CEIBObjectRecord()
{
}

CEIBObjectRecord::~CEIBObjectRecord()
{
}

void CEIBObjectRecord::AddRecord(unsigned char* value, unsigned char value_len)
{
	if ((int)_history.size() == MAX_NUM_OBJECT_HISTORY){
		_history.pop_back();
	}
	
	CEIBRecord rec;
	memcpy(rec._value,value,value_len);
	rec._time.SetNow();
	rec.SetValueLength(value_len);
	_history.push_front(rec);
}

void CEIBObjectRecord::Print(CString& str) const
{
	deque<CEIBRecord> tmp(_history);
	deque<CEIBRecord>::iterator it;
	for(it = tmp.begin(); it != tmp.end(); ++it){
		str += "Date: ";
		str += (*it)._time.Format();
		str += " Value: ";
		str += (*it).PrintValue();
		str += "\r\n";
	}
}

CEIBRecord::CEIBRecord()
{
}

CEIBRecord::CEIBRecord(const CEIBRecord& rhs):
_time(rhs._time),
_value_len(rhs._value_len)
{
	memcpy(_value,rhs._value,_value_len);
}

CEIBRecord::~CEIBRecord()
{
}

CString CEIBRecord::PrintValue() const
{
	CString ret("0x");
	for(int i =0; i < _value_len; ++i)
	{
		ret += ToHex((int)_value[i]);
	}
	return ret;
}
