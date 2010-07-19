#include "SMSServer.h"

void smsserver_main(bool interactive_conf, bool interactive_smsdb)
{
	if(interactive_conf){
		CSMSServer::GetInstance().InteractiveConf();
	}
	if(interactive_smsdb){
		CSMSServer::GetInstance().GetDB().InteractiveConf();
	}

	if(interactive_conf || interactive_smsdb){
		exit(0);
	}

	bool initialized = CSMSServer::GetInstance().Init();
	if(initialized)	{
		CSMSServer::GetInstance().start();
	}
	else{
		cerr << "Error during initialization of SMS Server." << endl;
	}

	CUtils::WaitForCharInput('q', "Press q to stop SMS Server: ");
	CSMSServer::GetInstance().Close();
}

int main(int argc, char **argv)
{
	JTCInitialize init;
	bool interactive_conf = false, interactive_smsdb = false;

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "iu :")) != -1)
	{
		switch(c)
		{
		case 'i': interactive_conf = true;
			break;
		case 'u': interactive_smsdb = true;
			break;
		}
	}

	smsserver_main(interactive_conf, interactive_smsdb);
	return 0;
}
