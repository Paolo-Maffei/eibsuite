#include "GenericTemplate.h"

const char* GetEIBUserName()
{
	return "username";
}

const char* GetPassword()
{
	return "password";
}

const char* GetEIBServerAddress()
{
	return "192.168.0.1";
}

const char* GetEncryptKey()
{
	return "EIB";
}

#ifdef WIN32
int GetLocalNetworkIf()
{
	return 0;
}
#else
const char* GetLocalNetworkIf()
{
	return "eth0";
}
#endif

void OnReceiveKNXMessage(unsigned short dstAddr, char* data, int data_len)
{
	//printf("Destination: %d, data:", dstAddr);
	int i;
	for(i=0; i<data_len;i++){
		//printf("%X",data[i]);
	}
	//printf("\n");
}
