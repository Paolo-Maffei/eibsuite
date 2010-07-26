/*! \file CString.h
    \brief String Class - Source file

	This is The header file for String class. This Class serves as the standard string representation for all
	other classed within the library. the class is based on the standard c++ string template class.

*/

#ifndef __STRING_HEADER__
#define __STRING_HEADER__

#include "EibStdLib.h"

#ifdef WIN32
#define strcasecmp _stricmp 
#endif

#define IS_HEX_DIGIT(c) (IS_DECIMAL_DIGIT_CHAR(c) || ((int)c >= 65 && (int)c <= 70) || ((int)c >= 97 && (int)c <= 102))
#define IS_DECIMAL_DIGIT_CHAR(c) ((int)c >= 48 && (int)c <= 57)

using namespace std;
/*!
	\brief A type definition for a __int64 type.
	\var typedef __int64 int64
*/
#ifdef WIN32
typedef __int64 int64;
#else
typedef int64_t int64;
#endif
/*!
	\brief A type definition for a unsigned __int64 type.
	\var typedef unsgined __int64 uint64
*/
#ifdef WIN32
typedef unsigned __int64 uint64;
#else
typedef uint64_t uint64;
#endif
/*! \class CString
	\brief String Class
*/
class EIB_STD_EXPORT CString
{
public:
	/*!Default Constructor*/
	CString();
	/*!Convert char* to CString Constructor*/
	CString(const char* str);
	/*!Convert char* with specified length to CString Constructor*/
	CString(const char* str,int length);
	/*!Convert unsigned char to CString Constructor*/
	CString(const unsigned char* str);
	/*!Convert unsigned char with specified length to CString Constructor*/
	CString(const unsigned char* str,int length);
	/*!Convert STL string to CString Constructor*/
	CString(const string& str);
	/* Convert int to CString Constructor*/
	CString(int str);
	/* Convert bool to CString Constructor - Camel case*/
	CString(bool str, bool camel_case = false);
	/* Convert unsigned int to CString Constructor*/
	CString(unsigned int str);
	/*!Convert int64 to CString Constructor*/
	CString(int64 str);
	/*!Convert double to CString Constructor*/
	CString(double str);
	/*!Convert double to CString Constructor*/
	CString(long str);
	/*!Convert double to CString Constructor*/
	CString(unsigned long str);
	/*! Copy Constructor*/
	CString(const CString& rhs);
	/*!Destructor*/
	virtual ~CString();

	/*!
	\fn const char& operator[](const int& index) const
	\return the character at the reuqested index. -1 if out of bounds.
	*/
	const char& operator[](const int& index) const;

	/*!
	\fn CString& operator=(const char* str)
	\return string representation of the passed value
	*/
	CString& operator=(const char* str);
	/*!
	\fn CString& operator=(const unsigned char* str)
	\return string representation of the passed value
	*/
	CString& operator=(const unsigned char* str);
	/*!
	\fn CString& operator=(const CString& rhs)
	\return string representation of the passed value
	*/
	CString& operator=(const CString& rhs);
	/*!
	\fn CString& operator=(int val)
	\return string representation of the passed value
	*/
	CString& operator=(int val);
	/*!
	\fn CString& operator=(unsigned int val)
	\return string representation of the passed value
	*/
	CString& operator=(unsigned int val);
	/*!
	\fn CString& operator=(short val)
	\return string representation of the passed value
	*/
	CString& operator=(short val);
	/*!
	\fn CString& operator=(unsigned short val)
	\return string representation of the passed value
	*/
	CString& operator=(unsigned short val);
	/*!
	\fn CString& operator=(double val)
	\return string representation of the passed value
	*/
	CString& operator=(double val);
	/*!
	\fn CString& operator=(int64 val)
	\return string representation of the passed value
	*/
	CString& operator=(int64 val);
	/*!
	\fn CString& operator=(uint64 val)
	\return string representation of the passed value
	*/
	CString& operator=(uint64 val);

	/*!
	\fn bool operator==(const CString& rhs) const
	\return true if both string are equal (case sensitive).
	*/
	bool operator==(const CString& rhs) const;
	/*!
	\fn bool operator==(const char* rhs) const
	\return true if both string are equal (case sensitive).
	*/
	bool operator==(const char* rhs) const;

	/*!
	\fn bool operator!=(const CString& rhs) const
	\return true if both string are NOT equal (case sensitive).
	*/
	bool operator!=(const CString& rhs) const;
	/*!
	\fn bool operator!=(const char* rhs) const
	\return true if both string are NOT equal (case sensitive).
	*/
	bool operator!=(const char* rhs) const;
	/*!
	\brief compare accroding to ASCII table values
	\fn bool operator<(const CString& rhs) const
	*/
	bool operator<(const CString& rhs) const;
	/*!
	\brief compare accroding to ASCII table values
	\fn bool operator<(const char* rhs) const
	\return true if both string are NOT equal (case sensitive).
	*/
	bool operator<(const char* rhs) const;
	/*!
	\brief compare accroding to ASCII table values
	\fn bool operator>(const CString& rhs) const
	\return true if both string are equal (case sensitive).
	*/
	bool operator>(const CString& rhs) const;
	/*!
	\brief compare accroding to ASCII table values
	\fn bool operator>(const char* rhs) const
	\return true if both string are NOT equal (case sensitive).
	*/
	bool operator>(const char* rhs) const;
	/*!
	\brief immutable operator
	\fn const CString operator+(char* rhs) const
	\return CString - the current string plus the characters added
	*/
	const CString operator+(char* rhs) const;
	/*!
	\brief immutable operator
	\fn const CString operator+(char* rhs) const
	\return CString - the current string plus the characters added
	*/
	const CString operator+(const char* rhs) const;
	/*!
	\brief immutable operator
	\fn const CString operator+(const CString& rhs) const
	\return CString - the current string plus the characters added
	*/
	const CString operator+(const CString& rhs) const;
	/*!
	\fn CString& operator+=(const CString& rhs)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const CString& rhs);
	/*!
	\fn CString& operator+=(const int& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const int& str);
	/*!
	\fn CString& operator+=(const char& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const char& str);
	/*!
	\fn CString& operator+=(const double& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const double& str);
	/*!
	\fn CString& operator+=(const unsigned int& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const unsigned int& str);
	/*!
	\fn CString& operator+=(const unsigned int& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const long& str);
	/*!
	\fn CString& operator+=(const char& c)
	\return CString - the current string plus the charcter added
	*/
	CString& operator+=(unsigned char c);
	/*!
	\fn CString& operator+=(const unsigned int& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const unsigned long& str);
	/*!
	\fn const CString operator+=(const int64& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const int64& str);
	/*!
	\fn const CString operator+=(const uint64& str)
	\return CString - the current string plus the charcters added
	*/
	CString& operator+=(const uint64& str);

	operator const char*() { return _str.c_str(); }
	/*!
	\brief retrun the underlying c++ string type. used in places where we must use it.
	\fn string& GetSTDString()
	\return string& - the underlying stl string type
	*/
	string& GetSTDString() { return _str;}
	/*!
	\brief Trim a string at the start according to specified character delimiter.
	\fn void TrimFromBeginning(char delim = ' ')
	\param delim - the character to trim the string according to.
	*/
	void TrimStart(char delim = ' ');
	/*!
	\brief Trim a string at the start & and end according to specified character delimiter.
	\fn void Trim(char delim = ' ')
	\param delim - the character to trim the string according to.
	*/
	void Trim(char delim = ' ');
	/*!
	\brief Get Method
	\fn int GetLength() const
	\return int the length of the string
	*/
	int GetLength() const;

	void Assign(const char* data) { _str.assign(data);}
	/*!
	\brief Substring method. returns substring of the currect string
	\fn CString SubString(int start,int length) const
	\param start - the position in the string to start the substring
	\param length - the length of the substring
	\return CString the requested substring
	*/
	CString SubString(int start,int length) const;
	/*!
	\brief Method to find a specific character withing a string.
	\fn int FindFirstOf(const char& c) const
	\param c - the character to search for
	\return int - the position of the searched char in the string. -1 if not found at all.
	*/
	int FindFirstOf(const char& c) const;
	/*!
	\brief Method change all of chracters in the current string to Lower case
	\fn void ToLower()
	\return void
	*/
	void ToLower();
	/*!
	\brief Method change all of chracters in the current string to Upper case
	\fn void void ToUpper()
	\return void
	*/
	void ToUpper();
	/*!
	\brief Method to find a given string within the current string.
	\fn int int FindFirstOf(const CString& c) const
	\param c - the string to search for
	\return int - the position of the searched string in the string. -1 if not found at all.
	*/
	int FindFirstOf(const CString& c) const;
	/*!
	\brief Method to search for a specific string within the currect string
	\fn int Find(const CString& value, int pos = 0) const
	\param value - the string to search for
	\param pos - the index within the current string to start the search from.
	\return int - the position of the searched string in the string. -1 if not found at all.
	*/
	int Find(const CString& value, int pos = 0) const;
	/*!
	\brief Method to search for a specific string within the currect string
	\fn int Find(const CString& value, int pos = 0) const
	\param value - the string to search for
	\param pos - the index within the current string to start the search from.
	\return int - the position of the searched string in the string. -1 if not found at all.
	*/
	int Find(char c, int pos = 0) const;
	/*!
	\brief Method to search for a specific string within the currect string. start the search from end to start (reverse)
	\fn int Find(const CString& value, int pos = 0) const
	\param value - the string to search for
	\param pos - the index within the current string to start the search from.
	\return int - the position of the searched string in the string. -1 if not found at all.
	*/
	int RFind(const CString& value, int pos = 0) const;
	/*!
	\brief Method to earse a part from a the currect string
	\fn void Erase(int start, int length)
	\param start - the position in the string to start the earse
	\param length - the length of the erase
	*/
	void Erase(int start, int length);
	/*!
	\brief Get Method
	\fn bool IsEmpty() const
	\return true if the string contains at least one character. false else
	*/
	bool IsEmpty() const;
	/*!
	\brief Method to clear the string from the contents. this method will cause the next call to "IsEmpty()" to return true.
	\fn void Clear()
	*/
	void Clear();
	/*!
	\brief Get the system char* pinter to the string
	\fn const char* GetBuffer() const
	\return const char* the pointer the the underlying system buffer.
	*/
	const char* GetBuffer() const;

	/*!
	\brief Get string value as char.
	\fn char ToChar() const
	\return char - string value as char
	*/
	char ToChar() const;
	/*!
	\brief Get string value as unsigned char.
	\fn unsigned char ToUChar() const
	\return unsigned char - string value as unsigned char
	*/
	unsigned char ToUChar() const;
	/*!
	\brief Get string value as int.
	\fn int ToInt() const
	\return int - string value as int
	*/
	int ToInt() const;
	/*!
	\brief Get string value as unsigned int
	\fn unsigned int ToUInt() const
	\return unsigned int - string value as unsigned int
	*/
	unsigned int ToUInt() const;
	/*!
	\brief Get string value as int64
	\fn int64 ToInt64() const
	\return int64 - string value as in64
	*/
	int64 ToInt64() const;
	/*!
	\brief Get string value as uint64
	\fn uint64 ToUInt64() const
	\return uint64 - string value as uint64
	*/
	uint64 ToUInt64() const;
	/*!
	\brief Get string value as double
	\fn double ToDouble() const
	\return double - string value as double
	*/
	double ToDouble() const;
	/*!
	\brief Get string value as boolean
	\fn bool ToBool() const
	\return bool - string value as boolean. (if string equals "True" then true. else false.
	*/
	bool ToBool() const;
	/*!
	\brief Get string value as short
	\fn short ToDouble() const
	\return short - string value as short
	*/
	short ToShort() const;
	/*!
	\brief Get string value as unsigned short
	\fn unsigned short ToUShort() const
	\return unsigned short - string value as unsigned short
	*/
	unsigned short ToUShort() const;
	/*!
	\brief Get string value as long
	\fn long ToLong() const
	\return long - string value as long
	*/
	long ToLong() const;
	/*!
	\brief Get string value as unsigned long
	\fn unsigned long ToULong() const
	\return unsigned long - string value as unsigned long
	*/
	unsigned long ToULong() const;

	bool EndsWith (const CString& str) const;
	bool EndsWith (const char* str) const;

	unsigned int HashCode() const;

	bool ShortFromHexString(short& val) const;
	bool UShortFromHexString(unsigned short& val) const;
	bool UCharFromHexString(unsigned char& val) const;
	bool BitFromHexString(unsigned char& val) const;
	bool IntFromHexString(int& val) const;
	bool UIntFromHexString(unsigned int& val) const;
	

	static CString ToHexFormat(char val);
	static CString ToHexFormat(unsigned char val);
	static CString ToHexFormat(short val);
	static CString ToHexFormat(unsigned short val);
	static CString ToHexFormat(int val);
	static CString ToHexFormat(unsigned int val);

private:
	static CString ToHexFormat(int n_left_padding, const CString& current);

private:
	string _str;
};
#endif
