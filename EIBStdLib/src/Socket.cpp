#include "Socket.h"

#ifdef WIN32
  typedef int socklen_t;
  typedef char raw_type;       // Type used for raw data on this platform
#else
  #include <sys/types.h>       // For data types
  #include <sys/socket.h>      // For socket(), connect(), send(), and recv()
  #include <netdb.h>           // For gethostbyname()
  #include <arpa/inet.h>       // For inet_addr()
  #include <unistd.h>          // For close()
  #include <netinet/in.h>      // For sockaddr_in
  #include <stdlib.h>
  typedef void raw_type;       // Type used for raw data on this platform
#endif

#include "time.h"
#include <errno.h>             // For errno
using namespace std;

#ifdef WIN32
#include <ws2tcpip.h>
#include <Iphlpapi.h>
static bool initialized = false;
#endif

//linux
#ifndef WIN32
typedef int SOCKET;
#endif

// SocketException Code
SocketException::SocketException(const CString &message, bool inclSysMsg)  throw() :
userMessage(message)
{
  if (inclSysMsg)
  {
    userMessage += ": (";
#ifdef WIN32
	int error_code = WSAGetLastError();
	char buf[256];
	strerror_s(buf,256,error_code);
	userMessage += error_code;
	userMessage += ") ";
    userMessage += buf;
	_error_code = error_code;
#else
	userMessage += strerror(errno);
#endif
  }
}

int SocketException::GetErrorCode() const throw()
{
	return _error_code;
}

SocketException::~SocketException() throw()
{
}

const char *SocketException::what() const throw()
{
  return userMessage.GetBuffer();
}

static void initSockFileDescriptors(SOCKET s, fd_set* rfds,struct timeval* tv, int timeout)
{
	FD_ZERO(rfds);
	FD_SET(s, rfds);
	if(timeout >= 1000){
		tv->tv_sec = (int)timeout / 1000;
	}
	else{
		//must be intialized in order to select work with usec precision
		tv->tv_sec = 0;
	}
	tv->tv_usec = (timeout % 1000) * 1000;
}

// Function to fill in address structure given an address and port
static void fillAddr(const CString &address, unsigned short port, sockaddr_in &addr)
{
	memset(&addr, 0, sizeof(addr));  // Zero out address structure
 	addr.sin_family = AF_INET;       // Internet address
	addr.sin_addr.s_addr = inet_addr(address.GetBuffer()); //*((unsigned long *) host->h_addr_list[0]);
  	addr.sin_port = htons(port);     // Assign port in network byte order
}

// Socket Code

Socket::Socket(int type, int protocol) throw(SocketException) {
  #ifdef WIN32
    if (!initialized) {
      WORD wVersionRequested;
      WSADATA wsaData;

      wVersionRequested = MAKEWORD(2, 0);              // Request WinSock v2.0
      if (WSAStartup(wVersionRequested, &wsaData) != 0) {  // Load WinSock DLL
        throw SocketException("Unable to load WinSock DLL");
      }
      initialized = true;
    }
  #endif

  // Make a new socket
  if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
    throw SocketException("Socket creation failed (socket())", true);
  }
}

Socket::Socket(int sockDesc) {
  this->sockDesc = sockDesc;
}

void Socket::GetError(CString& error_str)
{
#ifdef WIN32
	int error_num = WSAGetLastError();
	error_str = "Socket error: ";
	error_str += error_num;
#else
	//int error_num = errno;
	error_str = "Socket error: ";
	error_str += strerror(errno);
#endif
}

CString Socket::GetLocalAddress()
{
	CString res;
	struct sockaddr_in addr;
	int len = sizeof(sockaddr_in);
#ifdef WIN32
	if(getsockname(sockDesc,(struct sockaddr*)&addr,&len) < 0)
#else
	if(getsockname(sockDesc,(struct sockaddr*)&addr,(socklen_t*)&len) < 0)
#endif
	{
		throw SocketException("Unable to load Socket local address");
	}
	res = inet_ntoa(addr.sin_addr);
	return res;
}

#ifdef WIN32
CString Socket::LocalAddress(int interface_index) throw(SocketException)
{
    // Get adapter list
	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(
							AdapterInfo,// [out] buffer to receive data
							&dwBufLen); // [in] size of receive data buffer
  
  	//assert(dwStatus == ERROR_SUCCESS);  // Verify return value is valid, no buffer overflow

  	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to current adapter info
	
	do
  	{
		if(pAdapterInfo->Index == interface_index){
			return CString(pAdapterInfo->IpAddressList.IpAddress.String);
		}
		pAdapterInfo = pAdapterInfo->Next;    // Progress through linked list
  	}while(pAdapterInfo);

	CString err_msg = "Interface Index ";
	err_msg += interface_index;
	err_msg += " does not exist";
	throw SocketException(err_msg);
	return EMPTY_STRING;
}
#else
CString Socket::LocalAddress(const CString& interface_name) throw(SocketException)
{
	struct ifreq   buffer[32];
	struct ifconf  intfc;
	int fd;

	intfc.ifc_len = sizeof(buffer);
	intfc.ifc_buf = (char*) buffer;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		throw SocketException("Socket creation failed (socket())", true);
	}

	if (ioctl(fd, SIOCGIFCONF, &intfc) < 0)
	{
		throw SocketException("IOCTL Failed. (Get net interface configuration)", true);
	}

	struct ifreq *item = NULL;
	for(unsigned int i=0; i< intfc.ifc_len / sizeof(struct ifreq); i++)
	{
		item = &intfc.ifc_req[i];
		CString ifname = item->ifr_name;
		if(ifname == interface_name){
			CString ret(inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
			return ret;
		}
	}
	CString err = "Cannot find network interface with name \"";
	err += interface_name;
	err += "\"!!!";
	throw SocketException(err,false);
	return EMPTY_STRING;
}
#endif

void Socket::Close()
{
#ifdef WIN32
    ::closesocket(sockDesc);
 #else
    ::close(sockDesc);
#endif
	sockDesc = -1;
}

Socket::~Socket()
{
	Close();
}

Socket& Socket::operator=(const Socket &sock)
{
    sockDesc = sock.sockDesc;
    return *this;
}

int Socket::GetLocalPort() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local port failed (getsockname())", true);
  }
  return ntohs(addr.sin_port);
}

void Socket::SetLocalPort(int localPort) throw(SocketException) {
  // Bind the socket to its port
  sockaddr_in localAddr;
  memset(&localAddr, 0, sizeof(localAddr));
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(localPort);

  if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local port failed (bind())", true);
  }
}

void Socket::SetLocalAddressAndPort(const CString &localAddress,int localPort) throw(SocketException) {
  // Get the address of the requested host
  sockaddr_in localAddr;
  fillAddr(localAddress, localPort, localAddr);

  if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
	  CString error;
	  this->GetError(error);
	  throw SocketException("Set of local address and port failed (bind())", true);
  }
}

void Socket::CleanUp() throw(SocketException) {
  #ifdef WIN32
    if (WSACleanup() != 0) {
      throw SocketException("WSACleanup() failed");
    }
  #endif
}

unsigned short Socket::ResolveService(const CString &service,
                                      const CString &protocol) {
  struct servent *serv;        /* Structure containing service information */

  if ((serv = getservbyname(service.GetBuffer(), protocol.GetBuffer())) == NULL)
    return atoi(service.GetBuffer());  /* Service is port number */
  else
    return ntohs(serv->s_port);    /* Found port (network byte order) by name */
}

// CommunicatingSocket Code

CommunicatingSocket::CommunicatingSocket(int type, int protocol)
    throw(SocketException) : Socket(type, protocol) {
}

CommunicatingSocket::CommunicatingSocket(int newConnSD) : Socket(newConnSD) {
}

void CommunicatingSocket::Connect(const CString &foreignAddress,
    unsigned short foreignPort) throw(SocketException) {
  // Get the address of the requested host
  sockaddr_in destAddr;
  fillAddr(foreignAddress, foreignPort, destAddr);

  // Try to connect to the given port
  if (::connect(sockDesc, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
    throw SocketException("Connect failed (connect())", true);
  }
}

void CommunicatingSocket::Send(const void *buffer, int bufferLen)
    throw(SocketException) {
  if (::send(sockDesc, (raw_type *) buffer, bufferLen, 0) < 0)
  {
	  CString err;
	  GetError(err);
	  throw SocketException("Send failed (send())", true);
  }
}

int CommunicatingSocket::Recv(void *buffer, int bufferLen) throw(SocketException)
{
  int rtn;
  if ((rtn = ::recv(sockDesc, (raw_type *) buffer, bufferLen, 0)) < 0) {
    throw SocketException("Received failed (recv())", true);
  }

  return rtn;
}

CString CommunicatingSocket::GetForeignAddress()
    throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sockDesc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of foreign address failed (getpeername())", true);
  }
  return inet_ntoa(addr.sin_addr);
}

unsigned short CommunicatingSocket::GetForeignPort() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getpeername(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of foreign port failed (getpeername())", true);
  }
  return ntohs(addr.sin_port);
}

// TCPSocket Code

TCPSocket::TCPSocket()
    throw(SocketException) : CommunicatingSocket(SOCK_STREAM,
    IPPROTO_TCP) {
}


TCPSocket::TCPSocket(const CString &foreignAddress, unsigned short foreignPort)
    throw(SocketException) : CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
  Connect(foreignAddress, foreignPort);
}

TCPSocket::TCPSocket(int newConnSD) : CommunicatingSocket(newConnSD) {
}


int TCPSocket::Recv(void *buffer, int bufferLen,int time_out) throw(SocketException)
{
	if((unsigned)time_out == INFINITE){
		return CommunicatingSocket::Recv(buffer,bufferLen);
	}

	fd_set rfds;
    struct timeval tv;
    initSockFileDescriptors(sockDesc,&rfds,&tv,time_out);

	if (select(sockDesc + 1,&rfds,NULL,NULL,&tv) < 0){
		throw SocketException("Receive failed (recv())", true);
	}
	if(!FD_ISSET(sockDesc,&rfds)){
		return 0;
	}
	return CommunicatingSocket::Recv(buffer,bufferLen);
}
// TCPServerSocket Code

TCPServerSocket::TCPServerSocket(int localPort, int queueLen)
    throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
  SetLocalPort(localPort);
  SetListen(queueLen);
}

TCPServerSocket::TCPServerSocket(const CString &localAddress, int localPort, int queueLen)
    throw(SocketException) : Socket(SOCK_STREAM, IPPROTO_TCP) {
  SetLocalAddressAndPort(localAddress, localPort);
  SetListen(queueLen);
}

TCPSocket *TCPServerSocket::Accept(unsigned int time_out) throw(SocketException)
{
	int newConnSD;
	if(time_out == INFINITE)
	{
		if ((newConnSD = ::accept(sockDesc, NULL, 0)) < 0)
		{
			throw SocketException("Accept failed (accept())", true);
		}
	}
	else
	{
		fd_set rfds;
		struct timeval tv;
		initSockFileDescriptors(sockDesc,&rfds,&tv,time_out);

		if (select(sockDesc + 1,&rfds,NULL,NULL,&tv) < 0){
			CString error = "Accept() failed: ";
			CString tmp;
			GetError(tmp);
			error += tmp;
			throw SocketException(tmp.GetBuffer(), true);
		}
		if(!FD_ISSET(sockDesc,&rfds)){
			return NULL;
		}
		if ((newConnSD = ::accept(sockDesc, NULL, 0)) < 0)
		{
			throw SocketException("Accept failed (accept())", true);
		}

	}
	return new TCPSocket(newConnSD);
}


void Socket::SetNonBlocking() throw(SocketException) {
#ifdef WIN32
	unsigned long iMode = 1;
   ioctlsocket(sockDesc, FIONBIO, &iMode);
#else
	int flags = fcntl(sockDesc, F_GETFL, 0);
    fcntl(sockDesc, F_SETFL, flags | O_NONBLOCK );
#endif
}

void TCPServerSocket::SetListen(int queueLen) throw(SocketException) {
  if (listen(sockDesc, queueLen) < 0) {
    throw SocketException("Set listening socket failed (listen())", true);
  }
}

// UDPSocket Code

UDPSocket::UDPSocket() throw(SocketException) : CommunicatingSocket(SOCK_DGRAM,
    IPPROTO_UDP) {
  SetBroadcast();
}

UDPSocket::UDPSocket(unsigned short localPort)  throw(SocketException) :
    CommunicatingSocket(SOCK_DGRAM, IPPROTO_UDP) {
  SetLocalPort(localPort);
  SetBroadcast();
}

UDPSocket::UDPSocket(const CString &localAddress, unsigned short localPort)
     throw(SocketException) : CommunicatingSocket(SOCK_DGRAM, IPPROTO_UDP) {
  SetLocalAddressAndPort(localAddress, localPort);
  SetBroadcast();
}

void UDPSocket::SetBroadcast() {
  // If this fails, we'll hear about it when we try to send.  This will allow
  // system that cannot broadcast to continue if they don't plan to broadcast
  int broadcastPermission = 1;
  int res = setsockopt(sockDesc, SOL_SOCKET, SO_BROADCAST,(raw_type *) &broadcastPermission, sizeof(broadcastPermission));
  if(res < 0){
	  throw SocketException("Socket error", true);
  }
}

void UDPSocket::Disconnect() throw(SocketException) {
  sockaddr_in nullAddr;
  memset(&nullAddr, 0, sizeof(nullAddr));
  nullAddr.sin_family = AF_UNSPEC;

  // Try to disconnect
  if (::connect(sockDesc, (sockaddr *) &nullAddr, sizeof(nullAddr)) < 0) {
   #ifdef WIN32
    if (errno != WSAEAFNOSUPPORT) {
   #else
    if (errno != EAFNOSUPPORT) {
   #endif
      throw SocketException("Disconnect failed (connect())", true);
    }
  }
}

void UDPSocket::SendTo(const void *buffer, int bufferLen,
    const CString &foreignAddress, int foreignPort,int time_out)
{
	fd_set wfds;
	struct timeval tv;
	initSockFileDescriptors(sockDesc,&wfds,&tv,time_out);

	if (select(1,NULL,&wfds,&wfds,&tv) < 0){
		throw SocketException("Send failed (sendto())", true);
	}
	if(!FD_ISSET(sockDesc,&wfds))
	{
		return;
	}
	SendTo(buffer,bufferLen,foreignAddress,foreignPort);
}

void UDPSocket::SendTo(const void *buffer, int bufferLen,const CString &foreignAddress, int foreignPort) throw(SocketException)
{
  sockaddr_in destAddr;
  fillAddr(foreignAddress, foreignPort, destAddr);

  // Write out the whole buffer as a single message.
  if (sendto(sockDesc, (raw_type *) buffer, bufferLen, 0,(sockaddr *) &destAddr, sizeof(destAddr)) != bufferLen)
  {
	  CString error = "Send failed: ";
	  CString tmp;
	  GetError(tmp);
	  error += tmp;
	  throw SocketException(tmp.GetBuffer(), true);
  }
}

int UDPSocket::RecvFrom(void *buffer, int bufferLen, CString &sourceAddress,
	int &sourcePort, int time_out) throw(SocketException)
{
	if((unsigned)time_out == INFINITE){
		return RecvFrom(buffer,bufferLen,sourceAddress,sourcePort);
	}

	fd_set rfds;
	struct timeval tv;
	initSockFileDescriptors(sockDesc,&rfds,&tv,time_out);

	if (select(sockDesc + 1,&rfds,NULL,NULL,&tv) < 0){
		CString error = "Receive failed: ";
		CString tmp;
		GetError(tmp);
		error += tmp;
		throw SocketException(tmp.GetBuffer(), true);
	}
	if(!FD_ISSET(sockDesc,&rfds)){
		return 0;
	}
	return RecvFrom(buffer,bufferLen,sourceAddress,sourcePort);
}

int UDPSocket::RecvFrom(void *buffer, int bufferLen, CString &sourceAddress,int &sourcePort) throw(SocketException) {
  sockaddr_in clntAddr;
  socklen_t addrLen = sizeof(clntAddr);
  int rtn;
  if ((rtn = recvfrom(sockDesc, (raw_type *) buffer, bufferLen, 0,(sockaddr *) &clntAddr, (socklen_t *) &addrLen)) < 0)
  {
		CString error = "Receive failed: ";
		CString tmp;
		GetError(tmp);
		error += tmp;
		throw SocketException(tmp.GetBuffer(), true);
  }
  sourceAddress = inet_ntoa(clntAddr.sin_addr);
  sourcePort = ntohs(clntAddr.sin_port);

  return rtn;
}

void UDPSocket::SetMulticastTTL(unsigned char multicastTTL) throw(SocketException)
{
  if (setsockopt(sockDesc, IPPROTO_IP, IP_MULTICAST_TTL,(raw_type *) &multicastTTL, sizeof(multicastTTL)) < 0)
  {
    throw SocketException("Socket error", true);
  }
}

void UDPSocket::SetMulticastLoopBack(bool val) throw(SocketException)
{
	int flag = val ? 1 : 0;
	if(setsockopt( sockDesc, IPPROTO_IP, IP_MULTICAST_LOOP,	(raw_type *)&flag, sizeof(flag)) < 0)
	{
		throw SocketException("Socket error (set multicast loopback)", true);
	}
}

void UDPSocket::JoinGroup(const CString& local_address,const CString& multicastGroup) throw(SocketException)
{
  struct ip_mreq multicastRequest;

  multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.GetBuffer());
  multicastRequest.imr_interface.s_addr = inet_addr(local_address.GetBuffer());

  if (setsockopt(sockDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP,(raw_type *) &multicastRequest,sizeof(multicastRequest)) < 0)
  {
	  throw SocketException("Multicast group join failed (setsockopt())", true);
  }
}

void UDPSocket::LeaveGroup(const CString& multicastGroup) throw(SocketException) {
  struct ip_mreq multicastRequest;

  multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.GetBuffer());
  multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sockDesc, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                 (raw_type *) &multicastRequest,
                 sizeof(multicastRequest)) < 0) {
    throw SocketException("Multicast group leave failed (setsockopt())", true);
  }
}
