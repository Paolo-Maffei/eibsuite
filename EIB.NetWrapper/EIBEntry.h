#ifndef __EIB_ENTRY_HEADER__
#define __EIB_ENTRY_HEADER__

#include "CException.h"
#include "EIBAddress.h"
#include "EIBNetwork.h"

using namespace EibStack;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace EIBNetWrapper
{
	public ref class CEIBEntry
	{
	public:
		CEIBEntry();
		virtual ~CEIBEntry();

		property unsigned short Address
		{
			unsigned short get(){ return _address;}
			void set(unsigned short value) { _address = value;}
		}

		property array<Byte>^ Data
		{
			array<Byte>^ get(){ return _data;}
			void set(array<Byte>^ value) { _data = value;}
		}

	private:
		unsigned short _address;
		array<Byte>^ _data;
	};
}

#endif