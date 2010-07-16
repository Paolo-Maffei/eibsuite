#ifndef __EIB_CRI_CRD_HEADER__
#define __EIB_CRI_CRD_HEADER__

#include "CException.h"
#include "EIBNetIP.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CCRI_CRD
{
public:
	CCRI_CRD(unsigned char* data);
	CCRI_CRD(unsigned char connection_typecode,unsigned char* data);
	virtual ~CCRI_CRD();

	//get methods
	unsigned short GetConnectionTypeCode();
	unsigned short GetData();
	unsigned short GetLength();

	int GetDataSize() { return sizeof(_data);}
	unsigned char* ToByteArray() { return (unsigned char*)&_data;}

private:
	EIBNETIP_CRI_CRD _data;
};

}
#endif

