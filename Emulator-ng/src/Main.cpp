#include "Emulator-ng.h"

void emulator_main(bool interactive_conf)
{
	if(interactive_conf){
		CEIBEmulator::GetInstance().InteractiveConf();
		exit(0);
	}
	
	bool initialized = CEIBEmulator::GetInstance().Init();
	if(initialized){
		CEIBEmulator::GetInstance().Run(NULL);
	}
	else{
		cerr << "Error initializating EIB Emulator." << endl;
	}

	CEmulatorCmd::StartLoop();
	CEIBEmulator::GetInstance().Close();
	CEIBEmulator::GetInstance().Destroy();
}

int main(int argc, char **argv)
{
	bool interactive_conf = false;

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "i :")) != -1)
	{
		switch(c)
		{
		case 'i': interactive_conf = true;
			break;
		}
	}

	emulator_main(interactive_conf);
	return 0;
}
