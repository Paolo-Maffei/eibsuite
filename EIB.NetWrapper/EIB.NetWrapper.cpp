// This is the main DLL file.
#using <mscorlib.dll>
#include "EIB.NetWrapper.h"

using namespace EIBNetWrapper;
using namespace msclr::interop;

namespace EIBNetWrapper
{

	CGenericServerWrapper::CGenericServerWrapper() : _ptr(NULL)
	{
	}

	void CGenericServerWrapper::Init(String^ log_file_str)
	{
		if(!String::IsNullOrEmpty(log_file_str))
		{
			char* p_log_file = (char*)Marshal::StringToHGlobalAnsi(log_file_str).ToPointer();
			_log = new CLogFile();
			CString log_file_full_name(p_log_file);
			_log->Init(log_file_full_name);
			Marshal::FreeHGlobal((IntPtr)p_log_file);
		}
	}

	bool CGenericServerWrapper::IsConnected()
	{
		if(_ptr != NULL && _ptr->IsConnected()){
			return true;
		}
		return false;
	}

	CGenericServerWrapper::~CGenericServerWrapper()
	{
		if(_ptr != NULL){
			delete _ptr;
			_ptr = NULL;
		}

		if(_log != NULL){
			delete _log;
			_log = NULL;
		}
	}

	int CGenericServerWrapper::SendEIBNetwork(unsigned short function,unsigned char value, SendMode blocking_mode)
	{
		if (_ptr != NULL){
			CEibAddress addr(function,true);
			unsigned char val[1];
			val[0] = value;
			return _ptr->SendEIBNetwork(addr,val,1,(BlockingMode)blocking_mode);
		}
		return 0;
	}

	void CGenericServerWrapper::Close()
	{
		if (_ptr != NULL){
			_ptr->Close();
			delete _ptr;
			_ptr = NULL;
		}
	}

	int CGenericServerWrapper::ReceiveEIBNetwork(unsigned short& function,unsigned char& value,int timeout)
	{
		timeout = (timeout == System::Threading::Timeout::Infinite) ? INFINITE : timeout;
		
		if (_ptr != NULL){
			CEibAddress addr;
			unsigned char val[MAX_EIB_VALUE_LEN];
			unsigned char val_len = MAX_EIB_VALUE_LEN;
			int res = _ptr->ReceiveEIBNetwork(addr,val,val_len,timeout);
			value = val[0];
			return res;
		}
		return 0;
	}

	bool CGenericServerWrapper::OpenConnection(String^ network_name,
											   String^ eib_server_adress,
											   int eib_server_port,
											   String^ initial_key,
											   String^ local_ip,
											   String^ username,
											   String^ password)
	{
		if(_log == NULL){
			return false;
		}

		if(_ptr == NULL)
		{
			_ptr = new CGenericServer(_network_id);
		}

		_ptr->Init(_log);

		marshal_context  ctx;
		bool result = false;

		if(_ptr != NULL)
		{			
			result = _ptr->OpenConnection(ctx.marshal_as<const char*>(network_name),
										  ctx.marshal_as<const char*>(eib_server_adress),
										  eib_server_port,
										  ctx.marshal_as<const char*>(initial_key),
										  ctx.marshal_as<const char*>(local_ip),
										  ctx.marshal_as<const char*>(username),
										  ctx.marshal_as<const char*>(password));
		}

		return result;
	}

	int CGenericServerWrapper::ReceiveEIBNetwork(CEIBEntry^ entry, int timeout)
	{
		if (_ptr != NULL){
			CEibAddress addr;
			unsigned char val[MAX_EIB_VALUE_LEN];
			unsigned char val_len = MAX_EIB_VALUE_LEN;
			int res = _ptr->ReceiveEIBNetwork(addr,val,val_len,timeout);
			IntPtr managed_ptr(val);
			Marshal::Copy(managed_ptr,entry->Data,(int)0,(int)val_len);
			return res;
		}
		return 0;
	}

	int CGenericServerWrapper::SendEIBNetwork(CEIBEntry^ entry, SendMode blocking_mode)
	{
		if (_ptr != NULL){
			CEibAddress addr(entry->Address,true);
			int val_len = 0;
			//pin the managed array
			pin_ptr<Byte> ptr = &entry->Data[0];
			//get unmanaged pointer
			unsigned char* raw_ptr = ptr;
			int len =_ptr->SendEIBNetwork(addr,raw_ptr,entry->Data->Length,(BlockingMode)blocking_mode);
		}
		return 0;
	}
};