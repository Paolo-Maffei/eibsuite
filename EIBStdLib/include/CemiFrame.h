#ifndef __CEMI_FRAME_HEADER__
#define __CEMI_FRAME_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "cEMI.h"
#include "EIBAddress.h"

enum CEMI_FRAME_PRIORITY
{
	PRIORITY_SYSTEM = 0x00,	//cEMI Message with system priority
    PRIORITY_URGENT = 0x08,	//cEMI Message with urgent priority
    PRIORITY_NORMAL = 0x04,	//cEMI Message with normal priority
    PRIORITY_LOW = 0x0C		//cEMI Message with low priority
};

namespace EibStack
{

class EIB_STD_EXPORT CCemiFrame
{
public:
	CCemiFrame();

	CCemiFrame(unsigned char message_control,
			   CEibAddress& source_addr,
			   CEibAddress& dst_addr,
			   unsigned char* data,
			   int data_len);

	CCemiFrame(const CCemiFrame& rhs);
	CCemiFrame(unsigned char* data);
	virtual ~CCemiFrame();

	void Dump() const;

	int GetTotalSize() const;
	bool IsExtendedFrame() const;
	unsigned char GetMessageCode() const{ return _data.mc; }
	unsigned char GetCtrl1() const { return _data.ctrl1; }
	CEibAddress GetSourceAddress() const;
	CEibAddress GetDestAddress() const;
	bool IsRepeatedFrame() const;
	void SetRepeatFlag(bool val);
	CEMI_FRAME_PRIORITY GetPriority() const;
	bool GetAcknowledgeRequested() const;
	bool IsPositiveConfirmation() const;
	unsigned short GetValueLength() const{ return _data.apci_length; }
	unsigned char GetAPCI() const { return _data.apci; }
	
	void Parse(unsigned char* data);

	unsigned char GetHopCount() const;
	void FillBuffer(unsigned char* buffer, int max_length) const;
	void SetPriority(CEMI_FRAME_PRIORITY priority);
	void SetAcknowledgeRequest(bool val);
	void SetDestAddress(const CEibAddress& add);
	void SetSrcAddress(const CEibAddress& add) { _data.saddr = add.ToByteArray(); }
	void SetMessageControl(unsigned char mc) { _data.mc = mc; }
	void SetAddilLength(unsigned char addil_len) { _data.addil = addil_len; }
	void SetValue(unsigned char* val, unsigned char val_len);
	void SetCtrl1(unsigned char ctrl1) { _data.ctrl1 = ctrl1;}
	void SetCtrl2(unsigned char ctrl2) { _data.ctrl2 = ctrl2;}

	CCemiFrame& operator=(const CCemiFrame& rhs);
	void FillBufferWithFrameData(unsigned char* buffer, int max_length);
	void SetFrameFormatStandard();

private:
	void CopyAddilData(byte acpi_len,unsigned char* data);

private:
	CEMI_L_DATA_MESSAGE _data;
	unsigned char* _addil_data;
};

}

#endif
