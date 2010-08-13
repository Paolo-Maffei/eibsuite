#include "EIBServer.h"

using namespace std;

void eibserver_main(bool interactive_conf, bool interactive_usersdb)
{
	CEIBServer::Create();
	
	if(interactive_conf){
		CEIBServer::GetInstance().InteractiveConf();
		exit(0);
	}
	
	if(interactive_usersdb){
		CEIBServer::GetInstance().GetUsersDB().InteractiveConf();
		exit(0);
	}

	bool initialized = CEIBServer::GetInstance().Init();
	if(initialized){
		CEIBServer::GetInstance().Start();
	}
	else{
		cerr << endl << "Server Failed to start: initialization of EIB Server failed." << endl;
	}

	CUtils::WaitForCharInput('q', "Press q to stop EIB Server: ", true);
	CEIBServer::GetInstance().Close();
	CEIBServer::Destroy();
}

int main(int argc, char **argv)
{
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

	eibserver_main(interactive_conf, interactive_usersdb);
	return 0;
}
