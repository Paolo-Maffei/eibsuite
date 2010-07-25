#include "MsgTable.h"
#include "AMXServer.h"

CMsgTable::CMsgTable()
{
}

CMsgTable::~CMsgTable()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
CMsgsDB::CMsgsDB()
{
}

CMsgsDB::~CMsgsDB()
{
}

void CMsgsDB::OnReadParamComplete(CString& current_record, const CString& param,const CString& value)
{
	AMXENTRY left;
	EIBENTRY right;

	//TODO: 
	//1. parse the right side
	//2. check for errors
	//3. insert to table

	int p_index = param.FindFirstOf(':');
	int v_index = value.FindFirstOf(':');
	if (p_index == -1 || v_index == -1){
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s , Parameter \"%s\" is missing ':' seperator. Expected syntax: AMX_device:P/R bit = EIB_function:value. All values are in Hex",_file_name.GetBuffer(),param.GetBuffer());
	}

	CString amx_device_str = param.SubString(0,p_index);
	CString amx_pr_str = param.SubString(p_index + 1,param.GetLength() - p_index - 1);

	unsigned char device_id = 0;
	if(!amx_device_str.UCharFromHexString(device_id)){
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s , Parameter \"%s\" has bad AMX device id.  Expected syntax: AMX_device:P/R bit = EIB_function:value. All values are in Hex",_file_name.GetBuffer(),param.GetBuffer());
	}

	unsigned char device_pr_val = 0;
	if(!amx_pr_str.BitFromHexString(device_pr_val)){
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s , Parameter \"%s\" Press/Release value should be 0x0 or 0x1.  Expected syntax: AMX_device:P/R bit = EIB_function:value. All values must be in Hex form",_file_name.GetBuffer(),param.GetBuffer());
	}

	left._device_id = device_id;
	left._press = device_pr_val == 1 ? true : false;
	left._release = !left._press;

	CString eib_function_str = value.SubString(0,v_index);
	CString ebi_val_str = value.SubString(v_index + 1,value.GetLength() - v_index - 1);
	
	unsigned short eib_function = 0;
	if(!eib_function_str.UShortFromHexString(eib_function)){
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s",_file_name.GetBuffer());
	}

	unsigned char eib_val = 0;
	if(!ebi_val_str.UCharFromHexString(eib_val)){
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s",_file_name.GetBuffer());
	}
	
	right._function = eib_function;
	right._value = eib_val;

	if(!CAMXServer::GetInstance().GetMsgsTable().InsertEntry(left,right))
	{
		throw CEIBException(ConfigFileError,"Error in Configuration file: %s. File contains duplicate entries",_file_name.GetBuffer());
	}
}

void CMsgsDB::OnReadRecordComplete(CString& current_record)
{
}

void CMsgsDB::OnReadRecordNameComplete(CString& current_record, const CString& record_name)
{
	if(record_name == AMX_MSG_TABLE_GENERAL_BLOCK)
	{
		current_record = record_name;
	}
}

void CMsgsDB::OnSaveRecordStarted(const CString& record,CString& record_name, list<pair<CString,CString> >& param_values)
{
}

/////////////////////////////////////////////////////////////////////////////////

bool AMXENTRY::operator<(const AMXENTRY& rhs) const
{
	if(_device_id < rhs._device_id)return true;
	if(_device_id > rhs._device_id) return false;
	if(!_press && rhs._press)return true;
	return false;
}

bool AMXENTRY::operator>(const AMXENTRY& rhs) const
{
	if(_device_id < rhs._device_id)return false;
	if(_device_id > rhs._device_id) return true;
	if(_press && ! rhs._press)return true;
	return false;
}

bool AMXENTRY::operator==(const AMXENTRY& rhs) const
{
	if(_device_id < rhs._device_id)return false;
	if(_device_id > rhs._device_id) return false;
	if(_press != rhs._press)return false;
	return true;
}

bool EIBENTRY::operator<(const EIBENTRY& rhs) const
{
	if(_function < rhs._function) return true;
	if(_function > rhs._function) return false;
	if(_value < rhs._value) return true;
	return false;
}

bool EIBENTRY::operator>(const EIBENTRY& rhs) const
{
	if(_function < rhs._function) return false;
	if(_function > rhs._function) return true;
	if(_value > rhs._value) return true;
	return false;
}

bool EIBENTRY::operator==(const EIBENTRY& rhs) const
{
	return ((_function == rhs._function) && (_value == rhs._value));
}

