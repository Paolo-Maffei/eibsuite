#ifndef __EMULATOR_CMD_HEADER__
#define __EMULATOR_CMD_HEADER__

class CEmulatorCmd
{
public:
	CEmulatorCmd();
	virtual ~CEmulatorCmd();

	static void StartLoop();
	static void PrintAvailableCmds();
	static void HandleSendCommand();
};

#endif
