#ifndef __EIB_NET_IP_HEADER__
#define __EIB_NET_IP_HEADER__

#include "EibStdLib.h"
#include "cEMI.h"

#define EIB_PORT 3671
#define EIB_MULTICAST_ADDRESS "224.0.23.12"

/*
 * ************** common constants ***
 */

#define	EIBNETIP_VERSION_10		0x10
#define	HEADER_SIZE_10			0x06

/*
 * ************* eibnetip services ***
 */

#define	EIBNETIP_CORE			0x02
#define	EIBNETIP_DEVMGMT		0x03
#define	EIBNETIP_TUNNELING		0x04
#define	EIBNETIP_ROUTING		0x05
#define	EIBNETIP_REMLOG			0x06
#define	EIBNETIP_REMCONF		0x07
#define	EIBNETIP_OBJSRV			0x08

/*
 * ******** core eibnetip services ***
 */

#define	SEARCH_REQUEST				0x0201
#define	SEARCH_RESPONSE				0x0202
#define	DESCRIPTION_REQUEST			0x0203
#define	DESCRIPTION_RESPONSE		0x0204
#define	CONNECT_REQUEST				0x0205
#define	CONNECT_RESPONSE			0x0206
#define	CONNECTIONSTATE_REQUEST		0x0207
#define	CONNECTIONSTATE_RESPONSE	0x0208
#define	DISCONNECT_REQUEST			0x0209
#define	DISCONNECT_RESPONSE			0x020A

/*
 * ***** device management services ***
 */

#define	DEVICE_CONFIGURATION_REQUEST	0x0310
#define	DEVICE_CONFIGURATION_ACK		0x0311

/*
 * ************* tunneling services ***
 */

#define	TUNNELLING_REQUEST			0x0420
#define	TUNNELLING_ACK				0x0421
#define	TUNNEL_LINKLAYER			0x02
#define	TUNNEL_RAW					0x04
#define	TUNNEL_BUSMONITOR			0x80

/*
 * *************** routing services ***
 */

#define	ROUTING_INDICATION			0x0530
#define	ROUTING_LOST_MESSAGE		0x0531

/*
 * *************** connection types ***
 */

#define	DEVICE_MGMT_CONNECTION		0x03
#define	TUNNEL_CONNECTION			0x04
#define	REMLOG_CONNECTION			0x06
#define	REMCONF_CONNECTION			0x07
#define	OBJSVR_CONNECTION			0x08

/*
 * ******************** error codes ***
 */
/*
 * ************* common error codes ***
 */

#define	E_NO_ERROR					0x00
#define	E_HOST_PROTOCOL_TYPE		0x01
#define	E_VERSION_NOT_SUPPORTED		0x02
#define	E_SEQUENCE_NUMBER			0x04

/*
 * *** connect response status codes ***
 */

#define	E_CONNECTION_TYPE		0x22
#define	E_CONNECTION_OPTION		0x23
#define	E_NO_MORE_CONNECTIONS	0x24

/*
 * *** connectionstate_response status codes ***
 */

#define	E_CONNECTION_ID		0x21
#define	E_DATA_CONNECTION	0x26
#define	E_KNX_CONNECTION	0x27

/*
 * ******* tunneling connect_ack error codes ***
 */

#define E_TUNNELING_LAYER	0x29

/*
 * *********** description information block ***
 */

#define	DEVICE_INFO			0x01
#define	SUPP_SVC_FAMILIES	0x02
#define	MFR_DATA			0xFE

/*
 * ************************  medium codes ***
 */

#define	TP0		0x01
#define	TP1		0x02
#define	PL110	0x04
#define	PL132	0x08
#define	RF		0x10

/*
 * ************************ host protocol codes ***
 */

#define	IPV4_UDP	0x01
#define	IPV4_TCP	0x02

/*
 * *************** timeout constants in seconds ***
 */

#define	CONNECT_REQUEST_TIME_OUT				5
#define	CONNECTIONSTATE_REQUEST_TIME_OUT		10
#define	DEVICE_CONFIGURATION_REQUEST_TIME_OUT	10
#define	HEARTBEAT_REQUEST_TIME_OUT				120
#define TUNNELING_REQUEST_TIME_OUT				1

/*
 * ********************** eibnet/ip object type ***
 */

#define EIBNET_IP_OBJECT_TYPE	13

/*
 * ************** eibnet/ip property definitions ***
 */

#define	PID_PROJECT_INSTALLATION_ID			51
#define	PID_KNX_INDIVIDUAL_ADDRESSES		52
#define	PID_ADDITIONAL_INDIVIDUAL_ADDRESSES	53
#define	PID_CURRENT_IP_ASSIGNMENT_METHOD	54
#define	PID_IP_ASSIGNMENT_METHOD			55
#define	PID_IP_CAPABILITIES					56
#define	PID_CURRENT_IP_ADDRESS				57
#define	PID_CURRENT_SUBNET_MASK				58
#define	PID_CURRENT_DEFAULT_GATEWAY			59
#define	PID_IP_ADDRESS						60
#define	PID_SUBNET_MASK						61
#define	PID_DEFAULT_GATEWAY					62
#define	PID_DHCP_BOOTP_SERVER				63
#define	PID_MAC_ADDRESS						64
#define	PID_SYSTEM_SETUP_MULTICAST_ADDRESS	65
#define	PID_ROUTING_MULTICAST_ADDRESS		66
#define	PID_TTL								67
#define	PID_EIBNETIP_DEVICE_CAPABILITIES	68
#define	PID_EIBNETIP_DEVICE_STATE	        69
#define	PID_EIBNETIP_ROUTING_CAPABILITIES	70
#define	PID_PRIORITY_FIFO_ENABLED			71
#define	PID_QUEUE_OVERFLOW_TO_IP	        72
#define	PID_QUEUE_OVERFLOW_TO_KNX			73
#define	PID_MSG_TRANSMIT_TO_IP				74
#define	PID_MSG_TRANSMIT_TO_KNX				75
#define	PID_FRIENDLY_NAME					76

/*
 * *********** internet protocol constants ***
 */

#define EIBNETIP_PORT_NUMBER			3671

/*
 * ******************************************* typedefs ***
 */

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef struct EIB_STD_EXPORT{
    byte headersize;
    byte version;
    word servicetype;
    word totalsize;
} EIBNETIP_HEADER;

typedef struct EIB_STD_EXPORT{
    EIBNETIP_HEADER head;
    byte *data;
} EIBNETIP_PACKET;

typedef struct EIB_STD_EXPORT{
    byte structlength;
    byte hostprotocol;
    byte ip1;
    byte ip2;
    byte ip3;
    byte ip4;
    word port;
} EIBNETIP_HPAI;

typedef struct EIB_STD_EXPORT{
	byte  structlength;
	byte  connectiontypecode;
	byte  protocolindependentdata;               // these two fields should be variable length
	byte  protocoldependentdata;                 // but we don't use it anyway
} EIBNETIP_CRI_CRD;

typedef struct EIB_STD_EXPORT{
    byte structlength;
    byte descriptiontypecode;
    byte knxmedium;
    byte devicestatus;
    word eibaddress;
    word projectinstallationidentifier;
    byte serialnumber[6];
    byte multicastaddress[4];
    byte macaddress[6];
    byte name[30];
} EIBNETIP_DEVINF_DIB;


typedef struct EIB_STD_EXPORT{
    byte structlength;
    byte descriptiontypecode;
    byte descriptiontypecode1;
    byte descriptiontypecode2;
    byte descriptiontypecode3;
    byte descriptiontypecode4;
} EIBNETIP_SUPPFAM_DIB;

typedef struct EIB_STD_EXPORT{
    byte structlength;
    byte descriptiontypecode;
    word manufacturerID;
    byte *data;
} EIBNETIP_MANUFACTURER_DIB;

typedef struct EIB_STD_EXPORT{
    EIBNETIP_HPAI endpoint;
    EIBNETIP_DEVINF_DIB devicehardware;
    EIBNETIP_SUPPFAM_DIB supported;
    EIBNETIP_MANUFACTURER_DIB *manufacturer;
} EIBNETIP_SEARCH_RESPONSE;

typedef struct EIB_STD_EXPORT{
    EIBNETIP_DEVINF_DIB *devicehardware;
    EIBNETIP_SUPPFAM_DIB *supported;
} EIBNETIP_DESCRIPTION_RESPONSE;

typedef struct {
    byte channelid;
    byte status;
    EIBNETIP_HPAI dataendpoint;
    EIBNETIP_CRI_CRD crd;
} EIBNETIP_CONNECT_RESPONSE;

typedef struct {
    EIBNETIP_HPAI controlendpoint;
    EIBNETIP_HPAI dataendpoint;
    EIBNETIP_CRI_CRD crd;
} EIBNETIP_CONNECT_REQUEST;

typedef struct EIB_STD_EXPORT{
    byte channelid;
    byte status;
} EIBNETIP_CONNECTIONSTATE_RESPONSE;

typedef struct EIB_STD_EXPORT{
    byte channelid;
    byte reserved;
    EIBNETIP_HPAI controlendpoint;
} EIBNETIP_CONNECTIONSTATE_REQUEST;

typedef struct EIB_STD_EXPORT{
    byte channelid;
    byte status;
} EIBNETIP_DISCONNECT_RESPONSE;

typedef struct EIB_STD_EXPORT{
    byte channelid;
    byte reserved;
    EIBNETIP_HPAI controlendpoint;
} EIBNETIP_DISCONNECT_REQUEST;

typedef struct EIB_STD_EXPORT{
    byte structlength;
    byte channelid;
    byte sequencecounter;
    byte typespecific;
} EIBNETIP_COMMON_CONNECTION_HEADER;

typedef struct EIB_STD_EXPORT{
    EIBNETIP_COMMON_CONNECTION_HEADER header;
} EIBNETIP_TUNNELING_REUQEST;

typedef struct EIB_STD_EXPORT{
} EIBNETIP_ROUTING_INDICATION;

typedef struct EIB_STD_EXPORT{
	EIBNETIP_HPAI discoveryendpoint;
}EIBNETIP_SEARCH_REQUEST;

#pragma pack(pop)   /* restore original alignment from stack */

typedef EIBNETIP_COMMON_CONNECTION_HEADER EIBNETIP_TUNNELING_ACK;

#endif
