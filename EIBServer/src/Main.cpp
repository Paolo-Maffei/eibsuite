#include "EIBServer.h"

using namespace std;

void usage()
{
	cout << "EIBServer Version: " << "1.0.0" << " GPLV2" << endl;
	cout << "Usage: EIBServer [OPTION]" << endl;
	cout << "Available options:" << endl;
	cout << '\t' << "-i Interactive mode for creating the configuration file" << endl;
	cout << '\t' << "-i Interactive mode for creating the users database file" << endl;
	cout << '\t' << "-h prints this message and exit" << endl << endl;
	cout << "Report EIBServer bugs to yosig81@gmail.com" << endl << endl;
}

void eibserver_main(bool interactive_conf, bool interactive_usersdb)
{
	JTCInitialize init;
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
		JTCThread::currentThread()->sleep(2000);
		exit(1);
	}

	CUtils::WaitForCharInput('q', "Press q to stop EIB Server: ", true);
	CEIBServer::GetInstance().Close();
}

int main(int argc, char **argv)
{
	bool interactive_conf = false, interactive_usersdb = false;
	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "iuh :")) != -1)
	{
		switch(c)
		{
		case 'i': interactive_conf = true;
			break;
		case 'u': interactive_usersdb = true;
			break;
		case 'h':
			usage();
			exit(0);
			break;
		}
	}

	eibserver_main(interactive_conf, interactive_usersdb);
	return 0;
}
