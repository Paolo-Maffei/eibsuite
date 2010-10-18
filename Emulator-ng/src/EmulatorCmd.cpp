#include "EmulatorCmd.h"
#include "Utils.h"
#include "cli.h"
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
	LOG_SCREEN("s - Send indication to specific group\n");
	LOG_SCREEN("d - Disconnect any connected client\n");
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
		}else if(input == "s"){
			HandleSendCommand();
		}else if (input == "d"){
			CEIBEmulator::GetInstance().GetHandler().DisconnectClients();
		}else{
			LOG_SCREEN("Unknown command. [Press '?' for list of available commands]\n");
		}

	}
}

void CEmulatorCmd::HandleSendCommand()
{
	CEmulatorHandler& handler = CEIBEmulator::GetInstance().GetHandler();
	CEmulatorDB& db = CEIBEmulator::GetInstance().GetDB();

	int size = db.GetNumOfRecords();
	db.Print();

	CGroupEntry ge;
	int index;
	ConsoleCLI::GetIntRange("\nSelect Group: ", index, 1, size, 1);
	if(db.GetGroupEntryByIndex(index, ge)){
		handler.SendIndication(ge);
	}else{
		LOG_ERROR("\nCannot find group with index %d.", index);
	}
}
