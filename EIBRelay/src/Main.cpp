#include "EIBRelayServer.h"

void releyserver_main(bool interactive_conf)
{
	if(interactive_conf){
		CEIBRelayServer::GetInstance().InteractiveConf();
		exit(0);
	}
	
	bool initialized = CEIBRelayServer::GetInstance().Init();
	if(initialized){
		CEIBRelayServer::GetInstance().Run(NULL);
	}
	else{
		cerr << "Error initializating EIB Relay Server. EIBRelay Failed to start." << endl;
		exit(1);
	}

	CUtils::WaitForCharInput('q', "Press q to stop EIBRelay Server: ");
	CEIBRelayServer::GetInstance().Close();
	CEIBRelayServer::GetInstance().Destroy();
}

int main(int argc, char **argv)
{
	JTCInitialize init;
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

	releyserver_main(interactive_conf);
	return 0;
}
