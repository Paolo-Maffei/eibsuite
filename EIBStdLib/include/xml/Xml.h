#ifndef __GLOBAL_XML_HEADER__
#define __GLOBAL_XML_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "DataBuffer.h"
#include "xml/xpath_static.h"

#define DEFAULT_XML_DOCUMENT_ROOT "Root"

#define SET_VALUE_T(val) CString tmp(val);\
							  this->SetValue(tmp.GetBuffer());

/*
class CXmlElement;

template <class T>
void SetValue_T(const T& val,CXmlElement& elem)
{
	CString tmp(val);
	elem.SetValue(tmp.GetBuffer());
}
*/

class EIB_STD_EXPORT CXmlElement
{
public:
	CXmlElement();
	CXmlElement(const CXmlElement& rhs);
	virtual ~CXmlElement();

	CXmlElement FirstChildElement();
	CXmlElement FirstChildElement(const CString& elem_name);
	CXmlElement FirstChildElement(const char* elem_name);

	CXmlElement NextSibling(const char* elem_name);

	CXmlElement InsertChild(const char* elem_name);

	virtual void SetValue(const char* val);

	virtual void SetValue(bool val) { SET_VALUE_T(val) }
	virtual void SetValue(int val) { SET_VALUE_T(val) }
	virtual void SetValue(unsigned int val) { SET_VALUE_T(val) }
	virtual void SetValue(unsigned char val) { SET_VALUE_T(val) }
	virtual void SetValue(char val) { SET_VALUE_T(val) }
	virtual void SetValue(short val) { SET_VALUE_T(val) }
	virtual void SetValue(unsigned short val) { SET_VALUE_T(val) }
	virtual void SetValue(const CString& val) { SET_VALUE_T(val) }

	CString GetValue();



	bool IsValid() { return _elem != NULL; }

	friend class CXmlDocument;
	CXmlElement& operator=(const CXmlElement& rhs);

private:
	TiXmlNode* _elem;
};

class EIB_STD_EXPORT CXmlDocument
{
public:
	CXmlDocument();
	CXmlDocument(const CString& xml_str);
	CXmlDocument(const char* xml_str);
	virtual ~CXmlDocument();

	virtual CXmlElement RootElement();
	virtual void Parse(const char* xml_str);
	virtual void Parse(const CString& xml_str);
	virtual void ToString(CDataBuffer& buffer);


private:
	TiXmlDocument* _doc;
};

#endif
