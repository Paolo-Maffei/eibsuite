#include "EIBServer.h"

using namespace std;

void eibserver_main(bool interactive_conf)
{
	CEIBServer::Create();
	
	if(interactive_conf){
		CEIBServer::GetInstance().InteractiveConf();
	}
	
	bool initialized = CEIBServer::GetInstance().Init();
	if(initialized){
		CEIBServer::GetInstance().Start();
	}
	else{
		cerr << endl << "Server Failed to start: initialization of EIB Server failed." << endl;
		//start and close dummy thread to prevent exception from being thrown in JTCInitialize destructor
		//We do not need to delete the dummy object since the destructor is being called from the thread base class
		//CDummyThreadHandle dummy = new CDummyThread();
		//dummy->start();
		//dummy->join();
	}

	char x = (char)0 ;
	while (true)
	{
		cout << endl << "Press q to stop EIB Server: " << endl;
		cin >> x ;
		if(x != 'q'){
			cout << "Incorrect Choice." << endl;
			cin.ignore(INT_MAX,'\n');
		}
		else{
			break;
		}
	}

	CEIBServer::GetInstance().Close();
	CEIBServer::Destroy();
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

	eibserver_main(interactive_conf);
	return 0;
}
