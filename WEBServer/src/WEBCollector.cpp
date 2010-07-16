#include "WEBCollector.h"
#include "WEBServer.h"

CWebCollector::CWebCollector() :
_stop(false),
_schedule(ScheduledCommandComparison())
{
}

CWebCollector::~CWebCollector()
{
}

void CWebCollector::run()
{
	CWEBServer& server = CWEBServer::GetInstance();

	CEibAddress function;
	unsigned char value[10];
	unsigned char value_len = 0;

	CTime t;
	t.SetNow();
	
	CTime time_count;
	time_count.SetNow();
	time_count += 60;
	int len = 0;

	while(!_stop)
	{
		
		if(time_count.secTo() == 0){
			CheckForScheduledCommand();
			time_count.SetNow();
			time_count += 60;
		}

		len = server.ReceiveEIBNetwork(function,value,value_len,1000);

		if(len == 0){
			value_len = 0;
			continue;
		}

		CWEBServer::GetInstance().GetLog().Log(LOG_LEVEL_DEBUG,"[Received] CEMI frame from EIB Server (%d bytes).",len);

		//CString address = function.ToString();
		//CWEBServer::GetInstance().GetLog().Log(LOG_LEVEL_DEBUG,"func: %s val_len: %d", address.GetBuffer(), value_len);
		_db.AddRecord(function,value,value_len);
	}
}

void CWebCollector::Close()
{
	_stop = true;
}

void CWebCollector::CheckForScheduledCommand()
{
	JTCSynchronized sync(*this);

	if(_schedule.empty()){
		return;
	}

	ScheduledCommand cmd = _schedule.top();
	CTime t;
	t.SetNow();
	
	while(cmd._time <= t)
	{
		//remove
		_schedule.pop();
		//execute
		CWEBServer::GetInstance().SendEIBNetwork(cmd._dst, cmd._value, cmd._value_len, NON_BLOCKING);
		//get next
		if(_schedule.empty()){
			return;
		}
		cmd = _schedule.top();
	}
}

bool CWebCollector::AddScheduledCommand(const CTime& time,
										const CEibAddress& dst,
										unsigned char* value,
										unsigned char val_len,
										CString& err_str)
{
	JTCSynchronized sync(*this);
	
	CTime t;
	t.SetNow();
	if(time <= t){
		err_str += "Cannot schedule task in the past. a Time Machine should be used instead.";
		//cannot schedule command in the past...
		return false;
	}

	ScheduledCommand cmd = ScheduledCommand(time,dst,value,val_len);
	
	_schedule.push(ScheduledCommand(time,dst,value,val_len));

	return true;
}

CStatsDB CWebCollector::GetStatsDB()
{
	return _db;
}

void CWebCollector::ScheduleToHtml(CDataBuffer& content)
{
	ADD_TO_CONTENT(HTML_H2_OPEN_ADD_ATTRIBUTES);
	ADD_TO_CONTENT(HTML_ALIGN_LEFT);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_FONT_OPEN);
	ADD_TO_CONTENT("face=\"sans-serif\" color=\"blue\">");
	ADD_TO_CONTENT("Scheduled EIB Commands");
	ADD_TO_CONTENT(HTML_FONT_CLOSE);
	ADD_TO_CONTENT(HTML_H2_CLOSE);

	ADD_TO_CONTENT(HTML_TABLE_OPEN);
	ADD_TO_CONTENT(HTML_ALIGN_LEFT);
	ADD_TO_CONTENT(HTML_BORDER_1);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Address");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Time");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Value");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);

	SchedQueue temp = _schedule;
	
	while(temp.size() > 0)
	{
		ScheduledCommand cmd = temp.top();
		temp.pop();
		
		ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN_CENTERED);
		
		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		ADD_TO_CONTENT(HTML_FONT_OPEN);
		ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\" size=\"2\">");
		ADD_TO_CONTENT(cmd._dst.ToString());
		ADD_TO_CONTENT(HTML_FONT_CLOSE);		
		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);
		
		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		ADD_TO_CONTENT(HTML_FONT_OPEN);
		ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\" size=\"2\">");
		ADD_TO_CONTENT(cmd._time.Format());
		ADD_TO_CONTENT(HTML_FONT_CLOSE);		
		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);

		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		ADD_TO_CONTENT(HTML_FONT_OPEN);
		ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\" size=\"2\">");
		ADD_TO_CONTENT("0x");
		for(int i =0; i < cmd._value_len; ++i)
		{
			ADD_TO_CONTENT(ToHex((int)cmd._value[i]));
		}	

		ADD_TO_CONTENT(HTML_FONT_CLOSE);		
		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);

		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		//add "Remove" check box here...
		ADD_TO_CONTENT(HTML_TYPE_EQUALS_BUTTON);

		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);

		ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);
	}
	ADD_TO_CONTENT(HTML_TABLE_CLOSE);
}
