#ifndef __CEMI_HEADER__
#define __CEMI_HEADER__

/*
 * ************************ CEMI Message Codes ***
 */

#define	L_BUSMON_IND		0x2B
#define	L_RAW_IND			0x2D
#define	L_RAW_REQ			0x10
#define	L_RAW_CON			0x2F
#define	L_DATA_REQ			0x11
#define	L_DATA_CON			0x2E
#define	L_DATA_IND			0x29
#define	L_POLL_DATA_REQ		0x13
#define	L_POLL_DATA_CON		0x25
#define	M_PROP_READ_REQ		0xFC
#define	M_PROP_READ_CON		0xFB
#define	M_PROP_WRITE_REQ	0xF6
#define	M_PROP_WRITE_CON	0xF5
#define	M_PROP_INFO_IND		0xF7
#define	M_RESET_REQ			0xF1
#define	M_RESET_IND			0xF0

/*
 * ************************************ typedefinition ***
 */

typedef unsigned char byte;
typedef unsigned short word;


typedef struct EIB_STD_EXPORT{
    byte mc;
    byte ioth;
    byte iotl;
    byte oi;
    byte pid;
    word noe_six;
    byte *data;
} CEMI_MGMT_MESSAGE;

typedef struct EIB_STD_EXPORT{
    byte mc;
    byte addil;
    byte ctrl1;
    byte ctrl2;
    word saddr;
    word daddr;
    byte apci_length;
    byte tpci; //YGYG : ???? sould be removed ???
	byte apci;
} CEMI_L_DATA_MESSAGE;

typedef struct EIB_STD_EXPORT{
    byte mc;
    byte ctrl1;
    word saddr;			/*if message is a CON or REQ message this field is unused */
    word daddr;
    byte ctrl2;
    byte npdu;
} EMI_L_DATA_MESSAGE;

typedef struct EIB_STD_EXPORT{
    byte m_code;
    byte status;
    word timestamp;
    byte cfield;
    byte saddr;
    byte saddr1;
    byte daddr;
    byte daddr2;
    byte length;
    byte acpicontr;
} EMI_L_BUSMON_IND;

typedef struct EIB_STD_EXPORT{
    byte mc;
    byte length;
    word address;
} PC_GET_MESSAGE;

typedef CEMI_L_DATA_MESSAGE CemiFrame;
typedef EMI_L_DATA_MESSAGE EmiFrame;
typedef EMI_L_BUSMON_IND BusMonFrame;

#endif

/*
 * *************************************************** EOF ***
 */
