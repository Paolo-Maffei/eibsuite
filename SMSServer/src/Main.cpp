#include "SMSServer.h"

void smsserver_main(bool interactive_conf)
{
	//list<CUserAlertRecord> cmds;
	//bool res = CSMSServer::GetInstance().GetDB().FindSmsMesaages(4100,129,cmds);
	if(interactive_conf){
		CSMSServer::GetInstance().InteractiveConf();
		exit(0);
	}

	bool initialized = CSMSServer::GetInstance().Init();
	if(initialized)	{
		CSMSServer::GetInstance().start();
	}
	else{
		cerr << "Error during initialization of SMS Server." << endl;
	}
	//wait for connection establishment with EIB Server
	JTCThread::sleep(1000);

	char x = (char)0 ;
	while (true)
	{
		cout << endl << "Press q to stop SMS Server: ";
		cin >> x ;
		if(x != 'q'){
			cout << "Incorrect Choice." << endl;
			cin.ignore(INT_MAX,'\n');
		}
		else{
			break;
		}
	}

	CSMSServer::GetInstance().Close();
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

	smsserver_main(interactive_conf);
	return 0;
}
