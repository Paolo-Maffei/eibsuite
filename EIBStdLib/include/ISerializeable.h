#ifndef __INTERFACE_SERIALIZEABLE_HEADER__
#define __INTERFACE_SERIALIZEABLE_HEADER__

#include "EibStdLib.h"

template <class T>
class EIB_STD_EXPORT ISerializeable
{
public:
	ISerializeable(){};
	virtual ~ISerializeable(){};
};

#endif

