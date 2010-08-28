#ifndef __CCEMI_L_BUSMON_FRAME_H__
#define __CCEMI_L_BUSMON_FRAME_H__

#include "EibStdLib.h"
#include "CemiFrame.h"
#include "CException.h"
#include "cEMI.h"
#include "EIBAddress.h"
#include "CTime.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CCemi_L_BusMon_Frame : public ICemiFrame, public CEIBNetPacket<CEMI_L_BUSMON_IND>
{
public:
	//CCemi_L_BusMon_Frame();
	CCemi_L_BusMon_Frame(const CCemi_L_BusMon_Frame& rhs);
	CCemi_L_BusMon_Frame(unsigned char* data, int len);
	virtual ~CCemi_L_BusMon_Frame();

	void Dump() const;

	void Parse(unsigned char* data, int len);

	void FillBuffer(unsigned char* buffer, int max_length) const;
	virtual CCemi_L_BusMon_Frame& operator=(const CCemi_L_BusMon_Frame& rhs);

	virtual unsigned char GetMessageCode() const { return _data.mc; }
	virtual unsigned char* GetPayload() { return _data.raw; }
	virtual unsigned short GetPayloadLength() { return _data.rawlen; }
	virtual unsigned short GetStructLength() { return _data.rawlen + 9; }

	unsigned char GetSequenceNumber() const { return (_data.status & 0x07); }
	CTime GetTimeStamp() const { return CTime(_data.timestamp); }

	bool GetParityError() const { return (_data.status & 0x20) == 0x20; }
	bool GetBitError() const { return (_data.status & 0x40) == 0x40; }
	bool GetFrameError() const { return (_data.status & 0x80) == 0x80; }
	bool GetLostFlag() const { return (_data.status & 0x08) == 0x08; }

private:
	bool ReadStatus(int typeID, int len, unsigned char* addInfo);
	bool ReadTimestamp(int typeID, int len, unsigned char* addInfo);

};

}

#endif
