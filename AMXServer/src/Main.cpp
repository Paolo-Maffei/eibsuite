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
		exit(1);
	}
	CUtils::WaitForCharInput('q', "Press q to stop AMX Server: ", true);
	CAMXServer::GetInstance().Close();
	return 0;
}

