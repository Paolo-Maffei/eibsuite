#ifndef __MESSAGE_TABLE_HEADER__
#define __MESSAGE_TABLE_HEADER__

#include "CString.h"
#include "TranslationTable.h"

class CMsgTable : public CTanslationTable<unsigned short,CString>
{
public:
	CMsgTable();
	virtual ~CMsgTable();

};

#endif