#include "Dispatcher.h"
#include "WEBServer.h"

CDispatcher::CDispatcher() : 
_server_sock(NULL),
_num_handlers(NUM_HANDLERS),
_stop(false)
{
	for (int i=0; i < _num_handlers; ++i)
	{
		_handlers[i] = new CWebHandler();
	}
	this->setName("Dispatcher");
}

CDispatcher::~CDispatcher()
{
	if(_server_sock != NULL){
		delete _server_sock;
		_server_sock = NULL;
	}
}

void CDispatcher::Close()
{
	_stop = true;
	JTCSynchronized sync(*this);
	
	for (int i=0; i < _num_handlers; ++i)
	{
		_handlers[i]->Close();
	}
}

void CDispatcher::Init()
{
	CWEBServerConfig& conf = CWEBServer::GetInstance().GetConfig();
	try
	{
		if(_server_sock == NULL){
			_server_sock = new TCPServerSocket(conf.GetWEBServerPort());
			_server_address = Socket::LocalAddress(conf.GetListenInterface());
		}
	}
	catch (SocketException& e)
	{
		LOG_ERROR("Socket Error: %s", e.what());
		throw CEIBException(SocketError,"Error in port [%d] binding: %s",GetServerPort(),e.what());
		return;
	}
}

void CDispatcher::run()
{
	JTCSynchronized sync(*this);
	unsigned int counter = 0;
	
	//start the handlers
	for (int i=0; i < _num_handlers ; ++i){
		_handlers[i]->start();
	}

	//_server_sock->SetNonBlocking();

	while (!_stop)
	{
		TCPSocket* sock = _server_sock->Accept(2000);
		if (sock != NULL)
		{
			int id = counter++ % _num_handlers;
			_handlers[id]->AddToJobQueue(sock);
			_handlers[id]->Signal();
		}
	}
}

