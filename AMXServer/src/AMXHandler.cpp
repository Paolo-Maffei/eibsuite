#include "AMXHandler.h"
#include "AMXServer.h"

CAMXListener::CAMXListener() : 
_stop(false)
{
}

CAMXListener::~CAMXListener()
{
}

void CAMXListener::Close()
{
	JTCSynchronized sync(*this);

	_stop = true;
}

void CAMXListener::run()
{
	JTCSynchronized sync(*this);

	int len = 0;
	unsigned char buffer[100];
	
	cout << endl;
	ParseResult res;
	while (!_stop)
	{
		//get massage from device
		START_TRY
			len = _sock.Recv(buffer,sizeof(buffer),2000);
			res.length = 0;
			unsigned char* tmp_ptr = buffer;
			int tmp_len = len;
			while(ParseSingleAMXFrame(tmp_ptr,tmp_len,res)){
				HandleParsedFrame(res);
				tmp_ptr += res.length;
				tmp_len -= res.length;
			}
			//last frame in packet
			HandleParsedFrame(res);
		END_TRY_START_CATCH_SOCKET(e)
			CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_ERROR,"AMX Handler Socket Exception: ",e.what());
		END_CATCH
		
		//packet inspection
		while (len > 0)
		{
			MaintainAMXConnection(buffer,len);		
		}

		this->wait(10);
	}
}

void CAMXListener::SendAMXMessage(unsigned char device_id, bool press)
{
	unsigned char Press[10]={0xa5,0x21,0x01,0xe1,0x01,0x00,0x00,0x00,0x00,0x00};
	unsigned char Rel[10]={0xa5,0x22,0x01,0xe1,0x01,0x00,0x00,0x00,0x00,0x00};

	if (press)
	{
		CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_DEBUG,"Pressing on device %d",device_id);
		Press[7]=device_id;
		Press[8]=crc21[device_id*2];
		Press[9]=crc21[device_id*2+1];
		JTCSynchronized sync(_mon);
		_sock.Send(Press,sizeof(Press));
	}
	else
	{
		CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_DEBUG,"Releasing device %d",device_id);
		Rel[7]=device_id;
		Rel[8]=crc22[device_id*2];
		Rel[9]=crc22[device_id*2+1];
		JTCSynchronized sync(_mon);
		_sock.Send(Rel,sizeof(Rel));
	}
}

void CAMXListener::Init()
{
	CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_INFO,"Connecting to AMX Interface...");
	const CAMXServerConfig& conf = CAMXServer::GetInstance().GetConfig();
	START_TRY
		_sock.Connect(conf.GetAMXDeviceAddress(),conf.GetAMXDevicePort());
		//send init bytes
		const unsigned char init[10]={0xA5,0x27,0x01,0xE1,0x01,0x00,0x00,0xDA,0x40,0xA5};
		_sock.Send(init,sizeof(init));
		CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_INFO,"Connection successful.");
	END_TRY_START_CATCH_SOCKET(e)
		throw CEIBException(SocketError,"Connection Failed. Reason: %s",e.what());
	END_CATCH
}

bool CAMXListener::ParseSingleAMXFrame(unsigned char* buf, int length, ParseResult& res)
{
	if(length < 2 || buf[0] != 0xa5){
		res.is_valid = false;
		return false;
	}
	
	switch (buf[1])
	{
	case 0x6:
		res.is_valid = false;
		res.length = 16;
		break;
	case 0x5:
		res.is_valid = false;
		res.length = 12;
		break;
	case 0x15:
		res.is_valid = false;
		res.length = 9;
		break;
	case 0x23:
		res.is_valid = true;
		res.length = 10;
		res.channel_id = buf[7];
		res.on = true;
		break;
	case 0x24:
		res.is_valid = true;
		res.length = 10;
		res.channel_id = buf[7];
		res.on = false;
		break;
	default:
		res.is_valid = false;
		return false;
	}

	return (length - res.length) > 0;
}

void CAMXListener::HandleParsedFrame(const ParseResult& result)
{	
	if (!result.is_valid)
	{
		return;
	}
	
	AMXENTRY amx_entry;
	amx_entry._device_id = result.channel_id;
	amx_entry._press = result.on;
	amx_entry._release = !result.on;

	EIBENTRY eib_entry;
	if(CAMXServer::GetInstance().GetMsgsTable().FindByLeft(amx_entry,eib_entry))
	{
		CEibAddress addr(eib_entry._function, true);
		unsigned char val[1];
		val[0] = eib_entry._value;
		CAMXServer::GetInstance().SendEIBNetwork(addr, val, 1, NON_BLOCKING);
	}
}

void CAMXListener::MaintainAMXConnection(unsigned char* buffer, int& len)
{
	int i;
	if (buffer[1] == 0x05){
	if (len > 12){
			for (i=12; i<len; i++) buffer[i-12]=buffer[i];
		}
		len-=12;
	}
	if (buffer[1] == 0x06){
		JTCSynchronized sync(_mon);
		_sock.Send(macack, sizeof(macack));
		_sock.Send(ver2, sizeof(ver2));
		if (len > 16){
			for (i=16; i<len; i++) buffer[i-16]=buffer[i];
		}
		len-=16;
	}
	if (buffer[1] == 0x13){
		if (len > 9){
			for (i=9; i<len; i++) buffer[i-9]=buffer[i];
		}
		len-=9;
	}
	if (buffer[1] == 0x14)
	{
		if (len > 9){
			for (i=9; i<len; i++) buffer[i-9]=buffer[i];
		}
		len-=9;
	}
	if (buffer[1] == 0x15){
		if (len > 9){
			for (i=9; i<len; i++) buffer[i-9]=buffer[i];
		}
		len-=9;
	}
	if (buffer[1] == 0x16){
		JTCSynchronized sync(_mon);
		_sock.Send(ver1, sizeof(ver1));
		if (len > 9)
		{
			for (i=9; i<len; i++) buffer[i-9]=buffer[i];
		}
		len-=9;
	}
	if (buffer[1] == 0x23){
		if (len > 10){
			for (i=10; i<len; i++) buffer[i-10]=buffer[i];
		}
		len-=10;
	}
	if (buffer[1] == 0x24){
		if (len > 10){
			for (i=10; i<len; i++) buffer[i-10]=buffer[i];
		}
		len-=10;
	}
	if (buffer[1] == 0x52){
		JTCSynchronized sync(_mon);
		_sock.Send(ver1s, sizeof(ver1s));
		if (len > 13){
			for (i=13; i<len; i++) buffer[i-13]=buffer[i];
		}
		len-=13;
   }
}

