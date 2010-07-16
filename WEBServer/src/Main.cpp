#include "WEBServer.h"

void webserver_main(bool interactive_conf, const CString& conf_file)
{
	if(conf_file.GetLength() > 0){
		CWEBServer::GetInstance().SetConfFile(conf_file);
	}
	if(interactive_conf){
		CWEBServer::GetInstance().InteractiveConf();
	}
	
	bool initialized = CWEBServer::GetInstance().Init();
	if(initialized){
		CWEBServer::GetInstance().Run(NULL);
	}
	else{
		cerr << "Error during initialization of WEB Server." << endl;
	}
	//wait for connection establishment with EIB Server
	JTCThread::sleep(1000);

	char x = (char)0 ;
	while (true)
	{
		cout << endl << "Press q to stop WEB Server: " << endl;
		cin >> x ;
		if(x != 'q'){
			cout << "Incorrect Choice." << endl;
			cin.ignore(INT_MAX,'\n');
		}
		else{
			break;
		}
	}

	CWEBServer::GetInstance().Close();
	//CWEBServer::GetInstance().Destroy();
}

int main(int argc, char **argv)
{
	JTCInitialize init;
	bool interactive_conf = false;
	CString conf_file;

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "if:")) != -1)
	{
		switch(c)
		{
		case 'i': interactive_conf = true;
			break;
		case 'f': conf_file = optarg;
			break;
		}
	}

	webserver_main(interactive_conf, conf_file);
	return 0;
}

