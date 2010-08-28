#ifndef __CEMI_FRAME_HEADER__
#define __CEMI_FRAME_HEADER__

#include "EibStdLib.h"

namespace EibStack
{

class EIB_STD_EXPORT ICemiFrame
{
public:
	virtual unsigned char GetMessageCode() const = 0;
	virtual unsigned char* GetPayload() = 0;
	virtual unsigned short GetPayloadLength() = 0;
	virtual unsigned short GetStructLength() = 0;
};

}

#endif
