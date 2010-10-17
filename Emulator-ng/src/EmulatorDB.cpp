#include "EmulatorDB.h"

CGroupEntry::CGroupEntry()
{
}

CGroupEntry::~CGroupEntry()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEmulatorDB::CEmulatorDB()
{

}

CEmulatorDB::~CEmulatorDB()
{

}

void CEmulatorDB::OnReadParamComplete(CGroupEntry& current_record, const CString& param,const CString& value)
{

}

void CEmulatorDB::OnReadRecordComplete(CGroupEntry& current_record)
{
	//if()
}

void CEmulatorDB::OnReadRecordNameComplete(CGroupEntry& current_record, const CString& record_name)
{
	CString prefix();
        if(record_name.Find(prefix) == -1){
		 return;
	}
}

void CEmulatorDB::OnSaveRecordStarted(const CGroupEntry& record,CString& record_name, list<pair<CString, CString> >& param_values)
{

}
