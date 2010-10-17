#include "EmulatorCmd.h"
#include "Utils.h"
#include "Emulator-ng.h"

CEmulatorCmd::CEmulatorCmd()
{
}

CEmulatorCmd::~CEmulatorCmd()
{
}

void CEmulatorCmd::PrintAvailableCmds()
{
	LOG_SCREEN("\n");
	LOG_SCREEN("q - Quit the emulator program\n");
	LOG_SCREEN("p - Print the current Emulator database\n");
}

void CEmulatorCmd::StartLoop()
{
	static CString msg("Enter command: [Press '?' for list of available commands] ");
	static CString input;
	while(true)
	{
		CUtils::WaitForInput(input, msg, false);
		input.ToLower();
		if(input == "q"){
			break;
		}else if(input == "?"){
			PrintAvailableCmds();
		}else if(input == "p"){
			CEIBEmulator::GetInstance().GetDB().Print();
		}else{
			LOG_SCREEN("Unknown command. [Press '?' for list of available commands]\n");
		}

	}
}


