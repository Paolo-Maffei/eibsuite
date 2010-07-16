#include "CemiFrame.h"
#include "EibNetwork.h"

using namespace EibStack;

CCemiFrame::CCemiFrame(const CCemiFrame& rhs) :
_addil_data(NULL)
{
	_data = rhs._data;
	CopyAddilData(rhs._data.apci_length, rhs._addil_data);
}

CCemiFrame::CCemiFrame() : 
_addil_data(NULL)
{
	memset(&_data,0,sizeof(_data));
}

CCemiFrame::CCemiFrame(unsigned char message_control,
					   CEibAddress& source_addr,
					   CEibAddress& dst_addr,
			           unsigned char* data,
					   int data_len) :
_addil_data(NULL)
{
	if(message_control != L_DATA_REQ && message_control != L_DATA_CON && message_control != L_DATA_IND){
		throw CEIBException(GeneralError,"Unknown message code");
	}
	
	_data.mc = message_control;
	_data.addil = 0;
	_data.ctrl1 = 0;
    _data.ctrl2 = 0x60; // default: routing counter = 6
	
	SetPriority(PRIORITY_NORMAL);

	if (dst_addr.IsGroupAddress()){
        _data.ctrl2 |= 0x0080;
	}
	else{
		_data.ctrl2 &= ~0x0080;
	}
	
	SetFrameFormatStandard();

	ASSERT_ERROR(!source_addr.IsGroupAddress(),"Source address can't be of type group address");

	_data.saddr = source_addr.ToByteArray();
	_data.daddr = dst_addr.ToByteArray();

	ASSERT_ERROR((data_len >= 1),"Illegal cEMI data. Length should be at least 1 Byte");
	
	_data.apci_length = (unsigned char)data_len;
	_data.tpci = 0;
	_data.apci = data[0];

	CopyAddilData(data_len, &data[1]);

	ASSERT_ERROR(data_len <= MAX_EIB_VALUE_LEN,"Maximum allowable TPDU length is 6 octets (including APCI) in this implementation");
}

CCemiFrame::CCemiFrame(unsigned char* data) : _addil_data(NULL)
{
	Parse(data);
}

bool CCemiFrame::IsPositiveConfirmation() const
{
	if((_data.ctrl1 & 0x01) == 0x01){
		return false;
	}
	return true;
}

void CCemiFrame::SetDestAddress(const CEibAddress& add)
{
	_data.daddr = add.ToByteArray(); 
	if (add.IsGroupAddress()){
        _data.ctrl2 |= 0x0080;
	}
	else{
		_data.ctrl2 &= ~0x0080;
	}
}

void CCemiFrame::Parse(unsigned char* data)
{
	CEMI_L_DATA_MESSAGE *ptr = (CEMI_L_DATA_MESSAGE *)data;

	_data.mc = ptr->mc;

	ASSERT_ERROR(ptr->addil == 0,"Only cEMI packets with additional info length 0 are supported");
	_data.addil = ptr->addil;
	
	_data.ctrl1 = ptr->ctrl1;
	_data.ctrl2 = ptr->ctrl2;

	_data.saddr = htons(ptr->saddr);
	_data.daddr = htons(ptr->daddr);  

	_data.apci_length = ptr->apci_length;
	_data.tpci = ptr->tpci;
	_data.apci = ptr->apci;

	CopyAddilData(ptr->apci_length, &data[11]);
	//ASSERT_ERROR(!IsExtendedFrame(),"Only standard frames are supported");
}

CCemiFrame::~CCemiFrame()
{
	if (_addil_data != NULL){
		delete _addil_data;
		_addil_data = NULL;
	}
}

void CCemiFrame::FillBuffer(unsigned char* buffer, int max_length) const
{
	ASSERT_ERROR(max_length >= GetTotalSize(),"Buffer is too small");

	CEMI_L_DATA_MESSAGE *ptr = (CEMI_L_DATA_MESSAGE *)buffer;

	ptr->mc = _data.mc;
	ptr->addil = _data.addil;
	
	ASSERT_ERROR(ptr->addil == 0,"Only cEMI packets with additional info length 0 are supported");
	
	ptr->ctrl1 = _data.ctrl1;
	ptr->ctrl2 = _data.ctrl2;

	ptr->saddr = htons(_data.saddr);
	ptr->daddr = htons(_data.daddr);

	ptr->apci_length = _data.apci_length;
	ptr->tpci = _data.tpci;
	ptr->apci = _data.apci;
	
	memcpy(&buffer[11],_addil_data,_data.apci_length -1);
}

int CCemiFrame::GetTotalSize() const
{
	int res = (11 + _data.apci_length -1);
	return res;
}

void CCemiFrame::SetFrameFormatStandard()
{
	_data.ctrl1 |= 0x80;
    _data.ctrl2 &= 0xF0;
}

bool CCemiFrame::IsExtendedFrame() const
{
	if ((_data.ctrl1 & 0x0080) == 0){
		return true;
	}
    return false;
}

CEibAddress CCemiFrame::GetSourceAddress() const
{
	return CEibAddress((unsigned int)_data.saddr,false);
}

void CCemiFrame::SetValue(unsigned char* val, unsigned char val_len)
{
	if(val_len < 1 || val_len > MAX_EIB_VALUE_LEN){
		throw CEIBException(GeneralError,"Value length must be at least 1 and maximum 6");
	}

	/* YGYG - need to optimize this..[no need to delete anyway.]
	if(_data.apci_length > 1){
		if(_addil_data == NULL){
			throw CEIBException(GeneralError,"Current packet is corrupted");
		}
		delete _addil_data;
		_addil_data = NULL;
	}
	*/

	_data.apci_length = val_len;
	_data.apci = val[0];
	CopyAddilData(val_len, &val[1]);
}

CEibAddress CCemiFrame::GetDestAddress() const
{
	return CEibAddress((unsigned int)_data.daddr,(_data.ctrl2 & 0x0080) == 0x0080);
}

bool CCemiFrame::IsRepeatedFrame() const
{
	return ((_data.ctrl1 & 0x20) == 0);
}

void CCemiFrame::SetRepeatFlag(bool val)
{
	if (val){
		_data.ctrl1 &= ~0x20;
	}else{
		_data.ctrl1 |= 0x20;
	}
}

CEMI_FRAME_PRIORITY CCemiFrame::GetPriority() const
{
	return (CEMI_FRAME_PRIORITY)(_data.ctrl1 & 0x000C);
}

void CCemiFrame::SetPriority(CEMI_FRAME_PRIORITY priority) 
{
	_data.ctrl1 &= 0xFFF3;
    _data.ctrl1 |= (unsigned char)priority;
}

bool CCemiFrame::GetAcknowledgeRequested() const
{
	if ((_data.ctrl1 & 0x02) == 0){
		return false;
	}
    return true;
}

unsigned char CCemiFrame::GetHopCount() const
{
	return (unsigned char)((_data.ctrl2 & 0x70) >> 4);
}

void CCemiFrame::SetAcknowledgeRequest(bool val)
{
	if (val){
		_data.ctrl1 |= 0x0002;
	}else{
        _data.ctrl1 &= ~0x0002;
	}
}

CCemiFrame& CCemiFrame::operator=(const CCemiFrame& rhs)
{
	_data = rhs._data;
	CopyAddilData(rhs._data.apci_length, rhs._addil_data);
	return *this;
}

void CCemiFrame::FillBufferWithFrameData(unsigned char* buffer, int max_length)
{
	ASSERT_ERROR(max_length >= _data.apci_length, "Buffer is too small for packet data");

	buffer[0] = _data.apci;

	if(_data.apci_length > 1){
		memcpy(&buffer[1],_addil_data,_data.apci_length - 1);
	}
}

void CCemiFrame::CopyAddilData(byte acpi_len, unsigned char* data)
{
	if (acpi_len > 1)
	{
		if(_addil_data != NULL){
			delete _addil_data;
		}
		_addil_data = new unsigned char[acpi_len - 1];
		memcpy(_addil_data, data, acpi_len - 1);
	}
	else
	{
		_addil_data = NULL;
	}
}

void CCemiFrame::Dump() const
{
	//message code
	switch(_data.mc)
	{
	case L_DATA_REQ: printf("Message code: L_DATA_REQ (%#x)\n",_data.mc);
		break;
	case L_DATA_CON: printf("Message code: L_DATA_CON (%#x)\n",_data.mc);
		break;
	case L_DATA_IND: printf("Message code: L_DATA_IND (%#x)\n",_data.mc);
		break;
	default:
		break;
	}

	if(_data.mc == L_DATA_CON){
		printf("Confirmation: %s\n", IsPositiveConfirmation() ? "Positive\n" : "Negative\n");
	}
	
	printf("Source: %s\n",GetSourceAddress().ToString().GetBuffer());
	printf("Destination: %s\n",GetDestAddress().ToString().GetBuffer());
	
	printf("Priority: ");
	switch (GetPriority())
	{
	case PRIORITY_SYSTEM: printf("System (%#x)\n",PRIORITY_SYSTEM);
		break;
	case PRIORITY_URGENT: printf("PRIORITY_URGENT (%#x)\n",PRIORITY_URGENT);
		break;
	case PRIORITY_NORMAL: printf("PRIORITY_NORMAL (%#x)\n",PRIORITY_NORMAL);
		break;
	case PRIORITY_LOW: printf("PRIORITY_LOW (%#x)\n",PRIORITY_LOW);
		break;
	default: printf("UNKNOWN (%#x)\n",GetPriority());
		break;
	}
	printf("Hop count: %d\n",GetHopCount());
	printf("Is Ack requested: %s\n",GetAcknowledgeRequested() ? "true" : "false");
	printf("Is Repetition: %s\n",IsRepeatedFrame() ? "true" : "false");
	printf("TPDU: 0x");
	for(int i=0; i<_data.apci_length; ++i){
		if(i == 0)
			printf("%x",_data.apci);
		else
			printf("%x",_addil_data[i-1]);
	}
	printf("\n");
}

