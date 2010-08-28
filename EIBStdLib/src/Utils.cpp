#include "Utils.h"
#include "JTC.h"

void CUtils::ReadFile(const CString& full_name, CString& content)
{
	ifstream myfile;
	myfile.open(full_name.GetBuffer(),ios::in);
	if (myfile.fail()){
		content = EMPTY_STRING;
		return;
	}

	CString line;
	while (!myfile.eof())
	{
		getline(myfile,line.GetSTDString());
		content += line;
		content += LINE_BREAK;
	}
}

void CUtils::SaveFile(const CString& full_name, const CString& content)
{
}

int CUtils::GetFileSize(const CString& full_name)
{
	ofstream _file;
	_file.open(full_name.GetBuffer(), std::ios_base::binary | std::ios_base::in);
	if (!_file.good() || _file.eof() || !_file.is_open()) 
	{
		//error
	}
	_file.seekp(0, std::ios_base::beg);
	std::ifstream::pos_type begin_pos = _file.tellp();
	_file.seekp(0, std::ios_base::end);
	int size = static_cast<int>(_file.tellp() - begin_pos);
	_file.close();
	return size;
}

void CUtils::GetTimeStrForFile(CString& time_str)
{
	time_str.Clear();

	time_t raw_time;
	time(&raw_time);
	char buffer[80];
	struct tm* timeinfo = localtime(&raw_time);
	strftime (buffer,80,"%b_%d_%H_%M",timeinfo);

	time_str += buffer;
}

void CUtils::GetCurrentPath(CString& path)
{
	char current_path[_MAX_PATH];
	char* result = _getcwd(current_path,_MAX_PATH);
	if(result == NULL){
		throw CEIBException(SystemError,"Cant retreive the current path");
		//error
	}
	path = current_path;
	path += PATH_DELIM;
}

int CUtils::GetNumOfNICs()
{
#ifdef WIN32
	// Get adapter list
	/*
	LANA_ENUM AdapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&AdapterList;
	Ncb.ncb_length = sizeof(AdapterList);
	Netbios(&Ncb);
	return AdapterList.length;
	*/
	return 0;
#else

	struct ifconf ifc;
	char buff[1024];
	struct ifreq *ifr;
	int skfd;
	ifc.ifc_len = sizeof(buff);
	ifc.ifc_buf = buff;
	if ((skfd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		printf("new socket failed\n");
		exit(1);
	}
	if (ioctl(skfd, SIOCGIFCONF, &ifc) < 0) {
		printf("SIOCGIFCONF:Failed \n");
		exit(1);
	}
	ifr = ifc.ifc_req;
	return (ifc.ifc_len / sizeof(struct ifreq));
#endif
}

bool CUtils::EnumNics(map<CString,CString>& nics)
{
#ifdef WIN32
	// Get adapter list
	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(
							AdapterInfo,// [out] buffer to receive data
							&dwBufLen); // [in] size of receive data buffer
  
  	//assert(dwStatus == ERROR_SUCCESS);  // Verify return value is valid, no buffer overflow

  	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
	nics.clear();
	int i = 0;
  	do
  	{
		pair<CString,CString> p;
		p.first = CString(pAdapterInfo->Index);
		p.second += pAdapterInfo->Description; //pAdapterInfo->IpAddressList.IpAddress.String;
		nics.insert(nics.end(), p);
		pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
		++i;
  	}while(pAdapterInfo);

	return true;
#else
	char          buf[1024];
	struct ifconf ifc;
	struct ifreq *ifr;
	int           sck;
	int           nInterfaces;
	int           i;

	/* Get a socket handle. */
	sck = socket(AF_INET, SOCK_DGRAM, 0);
	if(sck < 0){
		return false;
	}
	/* Query available interfaces. */
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if(ioctl(sck, SIOCGIFCONF, &ifc) < 0){
		return false;
	}
	/* Iterate through the list of interfaces. */
	ifr         = ifc.ifc_req;
	nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
	nics.clear();
	for(i = 0; i < nInterfaces; i++)
	{
		struct ifreq *item = &ifr[i];
		pair<CString,CString> p;
		p.first = item->ifr_name;
		p.second = item->ifr_name;
		p.second += " (";
		p.second += inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr);
		p.second += ")";
		nics.insert(nics.end(), p);
	}
	return true;
#endif
}

void CUtils::WaitForCharInput(char expected, const CString& msg, bool sleep)
{
	if(sleep){
		//delay execution a bit, so we could flush any waiting buffers to screen before printing these msgs
		JTCThread::sleep(500);
	}

	char x = (char)0 ;
	while (true)
	{
		cout << endl << msg.GetBuffer() << endl;
		cin >> x ;
		if(x != expected){
			cout << "Incorrect Choice." << endl;
			cin.ignore(INT_MAX,'\n');
		}
		else{
			break;
		}
	}
}
