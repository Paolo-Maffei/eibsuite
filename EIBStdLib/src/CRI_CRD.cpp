#include "CRI_CRD.h"

using namespace EibStack;

CCRI_CRD::CCRI_CRD(unsigned char* data)
{
	ASSERT_ERROR(data[0] == 4, "wrong CRI_CRD length");

	_data.structlength = data[0];
    _data.connectiontypecode = data[1];
    _data.protocolindependentdata = data[2];
	_data.protocoldependentdata = data[3];
}

CCRI_CRD::CCRI_CRD(unsigned char connection_typecode,unsigned char* data)
{
	_data.structlength = sizeof(_data);
    _data.connectiontypecode = connection_typecode;
    _data.protocolindependentdata = data[0];
	_data.protocoldependentdata = data[1];
}

CCRI_CRD::~CCRI_CRD()
{
}

unsigned short CCRI_CRD::GetConnectionTypeCode()
{
	return _data.connectiontypecode;
}

unsigned short CCRI_CRD::GetData()
{
	unsigned short res = 0;
	unsigned char* ptr = (unsigned char*)&res;
	ptr[0] = _data.protocolindependentdata;
	ptr[1] = _data.protocoldependentdata;
	return res;
}

unsigned short CCRI_CRD::GetLength()
{
	return _data.structlength;
}
