#include "xml/Xml.h"

CXmlDocument::CXmlDocument(const char* xml_str) :
_doc(NULL)
{
	_doc = new TiXmlDocument();
	Parse(xml_str);
}

CXmlDocument::CXmlDocument() :
_doc(NULL)
{
	_doc = new TiXmlDocument();
	
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","UTF-8","no");
	_doc->InsertEndChild(*decl);
	delete decl;

	TiXmlElement* root_elem = new TiXmlElement(DEFAULT_XML_DOCUMENT_ROOT);
	_doc->InsertEndChild(*root_elem);
	delete root_elem;
}


CXmlDocument::CXmlDocument(const CString& xml_str) :
_doc(NULL)
{
	_doc = new TiXmlDocument();
	Parse(xml_str);
}

void CXmlDocument::Parse(const CString& xml_str)
{
	Parse(xml_str.GetBuffer());
}

void CXmlDocument::Parse(const char* xml_str)
{
	_doc->Parse(xml_str);
}


CXmlDocument::~CXmlDocument()
{
	if(_doc != NULL){
		delete _doc;
	}
}

CXmlElement CXmlDocument::RootElement()
{
	CXmlElement elem;
	elem._elem = _doc->LastChild(DEFAULT_XML_DOCUMENT_ROOT);
	return elem;
}

void CXmlDocument::ToString(CDataBuffer& buffer)
{
	TiXmlPrinter printer;
	printer.SetLineBreak("\r\n");
	printer.SetIndent("\t");
	_doc->Accept(&printer);
	buffer.Add(printer.CStr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CXmlElement::CXmlElement()
{
}

CXmlElement::CXmlElement(const CXmlElement& rhs)
{
	this->_elem = rhs._elem;
}

CXmlElement::~CXmlElement()
{
}

CXmlElement CXmlElement::FirstChildElement()
{
	TiXmlElement* temp = this->_elem->FirstChildElement();
	CXmlElement elem;
	elem._elem = temp;
	return elem;
}

CXmlElement& CXmlElement::operator=(const CXmlElement& rhs)
{
	this->_elem = rhs._elem;
	return *this;
}

CXmlElement CXmlElement::FirstChildElement(const CString& elem_name)
{
	return FirstChildElement(elem_name.GetBuffer());
}

CXmlElement CXmlElement::FirstChildElement(const char* elem_name)
{
	TiXmlElement* temp = this->_elem->FirstChildElement(elem_name);
	CXmlElement elem;
	elem._elem = temp;
	return elem;
}

CXmlElement CXmlElement::NextSibling(const char* elem_name)
{
	TiXmlElement* temp = this->_elem->NextSiblingElement(elem_name);
	CXmlElement elem;
	elem._elem = temp;
	return elem;
}

CString CXmlElement::GetValue()
{
	TiXmlText* temp = (TiXmlText*)_elem->FirstChild();
	CString ret;
	if(temp != NULL){
		ret = temp->Value();
	}
	return ret;
}

CXmlElement CXmlElement::InsertChild(const char* elem_name)
{
	TiXmlNode* temp = new TiXmlElement(elem_name);
	TiXmlElement* res = (TiXmlElement*)this->_elem->InsertEndChild(*temp);
	delete temp;

	CXmlElement inserted_elem;
	inserted_elem._elem = res;
	return inserted_elem;
}

void CXmlElement::SetValue(const char* val)
{
	TiXmlText* text = new TiXmlText(val);
	_elem->InsertEndChild(*text);
	delete text;
}

