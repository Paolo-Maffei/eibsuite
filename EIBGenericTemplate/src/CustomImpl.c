#include "GenericTemplate.h"
#include <stdio.h>

char* GetUserName()
{
	return "username";
}

char* GetPassword()
{
	return "password";
}

char* GetEIBServerAddress()
{
	return "192.168.0.1";
}

#ifdef WIN32
int GetNetworkIf()
{
	return 0;
}
#else
char* GetNetworkIf()
{
	return "eth0";
}
#endif

void OnReceiveKNXMessage(unsigned short from, unsigned short to, char* data, int data_len)
{
	printf("from: %d, to: %d, data:",from, to);
	int i;
	for(i=0; i<data_len;i++){
		printf("%X",data[i]);
	}
	printf("\n");
}
