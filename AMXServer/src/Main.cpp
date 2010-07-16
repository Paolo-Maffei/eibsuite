#include "AMXServer.h"

int main()
{
	bool initialized = CAMXServer::GetInstance().Init();
	if(initialized)
	{
		CAMXServer::GetInstance().Start();
	}
	else{
		cerr << "Error during initialization of AMX Server." << endl;
	}
	//wait for connection establishment with EIB Server
	JTCThread::sleep(1000);
	
	char x = (char)0 ;
	while (true)
	{
		cout << endl << "Press q to stop AMX Server: ";
		cin >> x ;
		if(x != 'q'){
			cout << "Incorrect Choice." << endl;
			cin.ignore(INT_MAX,'\n');
		}
		else{
			break;
		}
	}
	
	CAMXServer::GetInstance().Close();
	return 0;
}

