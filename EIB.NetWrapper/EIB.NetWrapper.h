// EIB.NetWrapper.h
#pragma once

#include "GenericServer.h"
#include "CException.h"
#include "EIBEntry.h"
#include <msclr/marshal.h>

using namespace System;
using namespace EibStack;

using namespace System::Runtime::InteropServices;


namespace EIBNetWrapper
{
	public ref class CGenericServerWrapper
	{
	public:
		CGenericServerWrapper();
		virtual ~CGenericServerWrapper();

		enum class SendMode : unsigned char
        {
            NonBlocking = NON_BLOCKING,
            WaitForAck = WAIT_FOR_ACK,
            WaitForConfirm = WAIT_FOR_CONFRM
        };

		void Init(String^ log_file_str);
		bool OpenConnection(String^ network_name, String^ eib_server_adress,int eib_server_port,String^ initial_key,
			     		    String^ local_ip,String^ username, String^ password);

		int ReceiveEIBNetwork(unsigned short& function,unsigned char& value, int timeout);
		int SendEIBNetwork(unsigned short function,unsigned char value, SendMode blocking_mode);

		int ReceiveEIBNetwork(CEIBEntry^ entry, int timeout);
		int SendEIBNetwork(CEIBEntry^ entry, SendMode blocking_mode);

		bool IsConnected();
		void Close();

		property unsigned char NetworkID
		{
			unsigned char get(){ return _network_id;}
			void set(unsigned char value) { _network_id = value;}
		}
		
		

	private:
		CGenericServer* _ptr;
		unsigned char _network_id;
		CLogFile* _log;
	};
}
