#include "SMSServer.h"


int main()
{
	bool initialized = CSMSServer::GetInstance().Init();
	
	list<CUserAlertRecord> cmds;
	bool res = CSMSServer::GetInstance().GetDB().FindSmsMesaages(4100,129,cmds);
	

	if(initialized)
	{
		CSMSServer::GetInstance().start();
		//wait for connection establishment with EIB Server
		JTCThread::sleep(1000);
	}
	else{
		cerr << "Error during initialization of SMS Server." << endl;
	}
	
	
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
	return 0;
}
