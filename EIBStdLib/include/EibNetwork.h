#ifndef __EIB_NETWORK_HEADER__
#define __EIB_NETWORK_HEADER__

#define EIB_TYPE_PM_SERVER		0x01
#define EIB_TYPE_SMS_SERVER 	0x02
#define EIB_TYPE_AMX_SERVER 	0x03
#define EIB_TYPE_EIB_SERVER		0x04
#define	EIB_TYPE_WEB_SERVER 	0x05
#define	EIB_TYPE_RELAY_SERVER 	0x06
#define	EIB_TYPE_GENERIC 		0x07

#define EIB_MSG_TYPE_KEEP_ALIVE			0x1
#define EIB_MSG_TYPE_KEEP_ALIVE_ACK		0x2
#define EIB_MSG_TYPE_EIB_CMD			0x3
#define EIB_MSG_TYPE_EIB_STATUS			0x4
#define EIB_MSG_TYPE_CMD_ACK			0x5
#define EIB_MSG_TYPE_CLINET_DISCONNECT	0x6 //When Client notifies the EIB Server about "Connection closed" event
#define EIB_MSG_TYPE_SERVER_DISCONNECT	0x7 //When EIB Server notifies the client about "Connection closed" event
#define EIB_MSG_TYPE_RELAY				0x8

#include <stdio.h>
#include <time.h>
#include "EIBNetIP.h"

#define MAX_EIB_VALUE_LEN 14

typedef struct tm tm;

enum EIB_STD_EXPORT EIB_DEVICE_MODE
{
	MODE_ROUTING,
	MODE_TUNNELING,
	MODE_BUSMONITOR,
	UNDEFINED_MODE
};

enum EIB_STD_EXPORT BlockingMode
{
	NON_BLOCKING = 1,
	WAIT_FOR_ACK = 2,
	WAIT_FOR_CONFRM = 3
};

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef struct EIB_STD_EXPORT EibNetworkHeader
{
	char _client_type;
	char _msg_type;
	char _mode;
}EibNetworkHeader;

typedef struct EIB_STD_EXPORT EIBInternalRelayMsg
{
	EibNetworkHeader _header;
	CEMI_L_DATA_MESSAGE _cemi_l_data_msg;
	unsigned char _addil[MAX_EIB_VALUE_LEN];
}InternalRelayMsg;

typedef struct EIB_STD_EXPORT EIBInternalNetMsg
{
	EibNetworkHeader _header;
	unsigned char _is_logical;
	unsigned short _function; //the original cemiFrame destination address
	unsigned char _value_len;
	unsigned char _value[MAX_EIB_VALUE_LEN];
}InternalNetMsg;

typedef struct EIB_STD_EXPORT ClientHeartBeatMsg
{
	EibNetworkHeader _header;
	int _session_id;
}ClientHeartBeatMsg;

typedef struct EIB_STD_EXPORT ClientAckMsg
{
	EibNetworkHeader _header;
	int _num_bytes_received;
}ClientAckMsg;

#pragma pack(pop)   /* restore original alignment from stack */

#endif
