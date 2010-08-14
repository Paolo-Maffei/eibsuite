#ifndef __EIB_ADDRESS_HEADER__
#define __EIB_ADDRESS_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "StringTokenizer.h"
#include "CException.h"

namespace EibStack
{

enum GROUP_LEVEL_FORMAT
{
	GROUP_2_LEVEL_FORMAT,
	GROUP_3_LEVEL_FORMAT
};

class EIB_STD_EXPORT CEibAddress
{
public:
	/**
     * 
     * This constructor is able to initialise the address object from its string
     * representation. The supported formats are:
     * <ul>
     * <li>zone.line.device (individual address, 4-4-8 bits)</li>
     * <li>main/middle/sub (3-level group address, 5-3-8 bits)</li>
     * <li>main/sub (2-level group address, 5-11 bits)</li>
     * </ul>
     * No range checking is performed.
     * 
     * @param _AddressString
     *            the address in string representation
     * @throws EICLException
     *             if another separator than "/" or "." is used or the number of
     *             elements (levels) is wrong
     * 
     */
	CEibAddress(const CString& address);

	/**
     * This constructor initializes a new EIB_Address object from the first two
     * bytes of a byte array, plus a Boolean value for its kind.
     * 
     * @param _Body
     *            the byte array
     * @param _IsGroupAddress
     *            indicates whether the address object is to be an individual
     *            (false, EIB_Address.INDIVIDUAL) or group address (true,
     *            EIB_Address.GROUP).
     */
    CEibAddress(unsigned char* body, bool is_group);

	/**
     * 
     * This constructor initializes a new EIB_Address object from a raw 16-bit
     * value. Although it would be possible to pass this value as a parameter of
     * type short, this would be cumbersome since all Java types are signed.
     * Therefore, this parameter is passed as an int.
     * 
     * @param address
     *            the raw address value
     * @param _IsGroupAddress
     *            indicates whether the address object is to be an individual
     *            (false, EIB_Address.INDIVIDUAL) or group address (true,
     *            EIB_Address.GROUP).
     */
	CEibAddress(unsigned int address, bool is_group);
	
	/**
     * Default (dummy) constructor. Initializes an EIB address instance of 0.0.0
     * (reserved individual address).
     * 
     */
    CEibAddress();
	CEibAddress(const CEibAddress& rhs);
	virtual ~CEibAddress();

	void Set(unsigned char* body, bool is_group);
	void Set(unsigned int address, bool is_group);

	bool IsGroupAddress() const;
	unsigned char GetZone() const;
	unsigned char GetLine() const;
	unsigned char GetDevice() const;
	unsigned char GetMainGroup() const;
	unsigned char GetMiddleGroup() const;
	unsigned short GetSubGroup11() const;
	unsigned char GetSubGroup8() const;

	unsigned short ToByteArray() const;

	CString ToString() const;

	bool operator==(const CEibAddress& rhs) const;
	bool operator!=(const CEibAddress& rhs) const;
	bool operator>(const CEibAddress& rhs) const;
	bool operator<(const CEibAddress& rhs) const;
	CEibAddress& operator=(const CEibAddress& rhs);

private:
	bool _is_logical;
	unsigned short _low;
	unsigned short _high;
	GROUP_LEVEL_FORMAT _group_type;

};

}

#endif
