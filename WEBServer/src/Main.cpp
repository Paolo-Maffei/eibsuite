#include "WEBServer.h"

void webserver_main(bool interactive_conf, bool interactive_usersdb)
{
	if(interactive_conf){
		CWEBServer::GetInstance().InteractiveConf();
		exit(0);
	}
	if(interactive_usersdb){
		CWEBServer::GetInstance().GetUsersDB().InteractiveConf();
		exit(0);
	}
	
	bool initialized = CWEBServer::GetInstance().Init();
	if(initialized){
		CWEBServer::GetInstance().Run(NULL);
	}
	else{
		cerr << "Error during initialization of WEB Server." << endl;
		exit(1);
	}
	//wait for connection establishment with EIB Server
	CUtils::WaitForCharInput('q', "Press q to stop WEB Server: ", true);
	CWEBServer::GetInstance().Close();
}

int main(int argc, char **argv)
{
	JTCInitialize init;
	bool interactive_conf = false, interactive_usersdb = false;

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "iu :")) != -1)
	{
		switch(c)
		{
		case 'i': interactive_conf = true;
			break;
		case 'u': interactive_usersdb = true;
			break;
		}
	}

	webserver_main(interactive_conf, interactive_usersdb);
	return 0;
}

