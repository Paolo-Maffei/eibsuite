#ifndef __CEMI_FRAME_L_DATA_HEADER__
#define __CEMI_FRAME_L_DATA_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "cEMI.h"
#include "CemiFrame.h"
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

class EIB_STD_EXPORT CCemi_L_Data_Frame : public ICemiFrame
{
public:
	CCemi_L_Data_Frame();

	CCemi_L_Data_Frame(unsigned char message_control,
			   const CEibAddress& source_addr,
			   const CEibAddress& dst_addr,
			   const unsigned char* data,
			   int data_len);

	CCemi_L_Data_Frame(const CCemi_L_Data_Frame& rhs);
	CCemi_L_Data_Frame(unsigned char* data);
	virtual ~CCemi_L_Data_Frame();

	void Dump() const;

	virtual unsigned char GetMessageCode() const { return _data.mc; }
	virtual unsigned char* GetPayload() { throw CEIBException(GeneralError, "Not implemented yet"); }
	virtual unsigned short GetPayloadLength()  { throw CEIBException(GeneralError, "Not implemented yet"); }
	virtual unsigned short GetStructLength()  { throw CEIBException(GeneralError, "Not implemented yet"); }

	int GetTotalSize() const;
	bool IsExtendedFrame() const;
	unsigned char GetCtrl1() const { return _data.ctrl1; }
	CEibAddress GetSourceAddress() const;
	CEibAddress GetDestAddress() const;
	bool IsRepeatedFrame() const;
	CEMI_FRAME_PRIORITY GetPriority() const;
	bool GetAcknowledgeRequested() const;
	bool IsPositiveConfirmation() const;
	unsigned short GetValueLength() const{ return _data.apci_length; }
	unsigned char GetAPCI() const { return _data.apci; }
	unsigned char GetTPCI() const { return _data.tpci; }
	const unsigned char* GetAddilData() const { return _addil_data; }
	unsigned char GetHopCount() const;
	
	void Parse(unsigned char* data);

	void FillBuffer(unsigned char* buffer, int max_length) const;
	void SetRepeatFlag(bool val);
	void SetPriority(CEMI_FRAME_PRIORITY priority);
	void SetAcknowledgeRequest(bool val);
	void SetDestAddress(const CEibAddress& add);
	void SetSrcAddress(const CEibAddress& add) { _data.saddr = add.ToByteArray(); }
	void SetMessageControl(unsigned char mc) { _data.mc = mc; }
	void SetAddilLength(unsigned char addil_len) { _data.addil = addil_len; }
	void SetValue(unsigned char* val, unsigned char val_len);
	void SetCtrl1(unsigned char ctrl1) { _data.ctrl1 = ctrl1;}
	void SetCtrl2(unsigned char ctrl2) { _data.ctrl2 = ctrl2;}
	void SetTPCI(unsigned char tpci) { _data.tpci = tpci; }
	void SetAPCI(unsigned char apci) { _data.apci = apci; }

	CCemi_L_Data_Frame& operator=(const CCemi_L_Data_Frame& rhs);
	void FillBufferWithFrameData(unsigned char* buffer, int max_length);
	void SetFrameFormatStandard();


private:
	void CopyAddilData(byte acpi_len, const unsigned char* data);

private:
	CEMI_L_DATA_MESSAGE _data;
	unsigned char* _addil_data;
};

}

#endif
