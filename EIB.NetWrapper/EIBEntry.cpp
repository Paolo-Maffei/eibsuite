#include "EIBEntry.h"

using namespace EIBNetWrapper;

CEIBEntry::CEIBEntry()
{
	_data = gcnew array<Byte>(MAX_EIB_VALUE_LEN);
}

CEIBEntry::~CEIBEntry()
{
}