#ifndef __CONF_BASE_HEADER__
#define __CONF_BASE_HEADER__

#include "CString.h"
#include "DataBuffer.h"
#include "xml/Xml.h"

#define APPEND_XML_NODE(xml_doc,node_name,node_value) {CXmlNode __node(node_name);\
													  xml_doc.GetRootElement().AddChild(__node);\
													  __node.AddValue(node_value);}

class IConfBase
{
public:
	IConfBase() {};
	virtual ~IConfBase(){};

	virtual void ToXml(CDataBuffer& xml_str) = 0;
	virtual void FromXml(const CDataBuffer& xml_str) = 0;

protected:
	CXmlDocument _doc;
};

#endif

