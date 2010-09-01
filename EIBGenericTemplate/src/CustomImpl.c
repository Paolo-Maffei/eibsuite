#include "GenericTemplate.h"
#include <stdio.h>

const char* GetUserName()
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
int GetNetworkIf()
{
	return 0;
}
#else
const char* GetNetworkIf()
{
	return "eth0";
}
#endif

void OnReceiveKNXMessage(unsigned short dstAddr, char* data, int data_len)
{
	printf("Destination: %d, data:", dstAddr);
	int i;
	for(i=0; i<data_len;i++){
		printf("%X",data[i]);
	}
	printf("\n");
}
