/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __PRACTICALSOCKET_INCLUDED__
#define __PRACTICALSOCKET_INCLUDED__

#include "EibStdLib.h"
#include "CString.h"
#include "Globals.h"

using namespace std;

/**
 *   Signals a problem with the execution of a socket call.
 */
class EIB_STD_EXPORT SocketException : public exception
{
public:
  /**
   *   Construct a SocketException with a explanatory message.
   *   @param message explanatory message
   *   @param inclSysMsg true if system message (from strerror(errno))
   *   should be postfixed to the user provided message
   */
  SocketException(const CString &message, bool inclSysMsg = false) throw();

  /**
   *   Provided just to guarantee that no exceptions are thrown.
   */
  virtual ~SocketException() throw();

  /**
   *   Get the exception message
   *   @return exception message
   */
  const char *what() const throw();
  /**
   *   Get the exception error code
   *   @return exception error code
   */
  int GetErrorCode() const throw();

private:
  CString userMessage;  // Exception message
  int _error_code;
};

/**
 *   Base class representing basic communication endpoint
 */
class EIB_STD_EXPORT Socket {
public:
  /**
   *   Close and deallocate this socket
   */
  ~Socket();

  /**
   *   Get the local address
   *   @return local address of socket
   *   @exception SocketException thrown if fetch fails
   */
  //CString GetLocalAddress() throw(SocketException);

  void Close();
	
  CString GetLocalAddress();
#ifdef WIN32
  static CString LocalAddress(int interface_index = 0) throw (SocketException);
#else
  static CString LocalAddress(const CString& interface_name = EMPTY_STRING) throw (SocketException);
#endif
  /**
   *   Get the local port
   *   @return local port of socket
   *   @exception SocketException thrown if fetch fails
   */
  int GetLocalPort() throw(SocketException);

  /**
   *   Set the local port to the specified port and the local address
   *   to any interface
   *   @param localPort local port
   *   @exception SocketException thrown if setting local port fails
   */
  void SetLocalPort(int localPort) throw(SocketException);

  /**
   *   Set the local port to the specified port and the local address
   *   to the specified address.  If you omit the port, a random port
   *   will be selected.
   *   @param localAddress local address
   *   @param localPort local port
   *   @exception SocketException thrown if setting local port or address fails
   */
  void SetLocalAddressAndPort(const CString &localAddress,int localPort = 0) throw(SocketException);

  /**
   *   If WinSock, unload the WinSock DLLs; otherwise do nothing.  We ignore
   *   this in our sample client code but include it in the library for
   *   completeness.  If you are running on Windows and you are concerned
   *   about DLL resource consumption, call this after you are done with all
   *   Socket instances.  If you execute this on Windows while some instance of
   *   Socket exists, you are toast.  For portability of client code, this is
   *   an empty function on non-Windows platforms so you can always include it.
   *   @exception SocketException thrown WinSock clean up fails
   */
  static void CleanUp() throw(SocketException);

  /**
   *   Resolve the specified service for the specified protocol to the
   *   corresponding port number in host byte order
   *   @param service service to resolve (e.g., "http")
   *   @param protocol protocol of service to resolve.  Default is "tcp".
   */
  static unsigned short ResolveService(const CString &service,
                                       const CString &protocol = "tcp");


  void SetNonBlocking() throw(SocketException);

  void GetError(CString& error_str);

private:

	Socket(const Socket &sock) { sockDesc = sock.sockDesc; };
    Socket& operator=(const Socket &sock);


protected:
  int sockDesc;              // Socket descriptor
  Socket(int type, int protocol) throw(SocketException);
  Socket(int sockDesc);
};

/**
 *   Socket which is able to connect, send, and receive
 */
class EIB_STD_EXPORT CommunicatingSocket : public Socket {
public:
  /**
   *   Establish a socket connection with the given foreign
   *   address and port
   *   @param foreignAddress foreign address (IP address or name)
   *   @param foreignPort foreign port
   *   @exception SocketException thrown if unable to establish connection
   */
  void Connect(const CString &foreignAddress, unsigned short foreignPort)
    throw(SocketException);

  /**
   *   Write the given buffer to this socket.  Call connect() before
   *   calling send()
   *   @param buffer buffer to be written
   *   @param bufferLen number of bytes from buffer to be written
   *   @exception SocketException thrown if unable to send data
   */
  void Send(const void *buffer, int bufferLen) throw(SocketException);

  /**
   *   Read into the given buffer up to bufferLen bytes data from this
   *   socket.  Call connect() before calling recv()
   *   @param buffer buffer to receive the data
   *   @param bufferLen maximum number of bytes to read into buffer
   *   @return number of bytes read, 0 for EOF, and -1 for error
   *   @exception SocketException thrown if unable to receive data
   */
  int Recv(void *buffer, int bufferLen) throw(SocketException);

  /**
   *   Get the foreign address.  Call connect() before calling recv()
   *   @return foreign address
   *   @exception SocketException thrown if unable to fetch foreign address
   */
  CString GetForeignAddress() throw(SocketException);

  /**
   *   Get the foreign port.  Call connect() before calling recv()
   *   @return foreign port
   *   @exception SocketException thrown if unable to fetch foreign port
   */
  unsigned short GetForeignPort() throw(SocketException);

protected:
  CommunicatingSocket(int type, int protocol) throw(SocketException);
  CommunicatingSocket(int newConnSD);
};

/**
 *   TCP socket for communication with other TCP sockets
 */
class EIB_STD_EXPORT TCPSocket : public CommunicatingSocket {
public:
  /**
   *   Construct a TCP socket with no connection
   *   @exception SocketException thrown if unable to create TCP socket
   */
  TCPSocket() throw(SocketException);

  /**
   *   Construct a TCP socket with a connection to the given foreign address
   *   and port
   *   @param foreignAddress foreign address (IP address or name)
   *   @param foreignPort foreign port
   *   @exception SocketException thrown if unable to create TCP socket
   */
  TCPSocket(const CString &foreignAddress, unsigned short foreignPort)
      throw(SocketException);

  int Recv(void *buffer, int bufferLen,int timeout) throw(SocketException);

private:
  // Access for TCPServerSocket::accept() connection creation
  friend class TCPServerSocket;
  TCPSocket(int newConnSD);
};

/**
 *   TCP socket class for servers
 */
class EIB_STD_EXPORT TCPServerSocket : public Socket {
public:
  /**
   *   Construct a TCP socket for use with a server, accepting connections
   *   on the specified port on any interface
   *   @param localPort local port of server socket, a value of zero will
   *                   give a system-assigned unused port
   *   @param queueLen maximum queue length for outstanding
   *                   connection requests (default 5)
   *   @exception SocketException thrown if unable to create TCP server socket
   */
  TCPServerSocket(int localPort, int queueLen = 5)
      throw(SocketException);

  /**
   *   Construct a TCP socket for use with a server, accepting connections
   *   on the specified port on the interface specified by the given address
   *   @param localAddress local interface (address) of server socket
   *   @param localPort local port of server socket
   *   @param queueLen maximum queue length for outstanding
   *                   connection requests (default 5)
   *   @exception SocketException thrown if unable to create TCP server socket
   */
  TCPServerSocket(const CString &localAddress, int localPort,
      int queueLen = 5) throw(SocketException);

  /**
   *   Blocks until a new connection is established on this socket or error
   *   @return new connection socket
   *   @exception SocketException thrown if attempt to accept a new connection fails
   */
  TCPSocket *Accept(unsigned int time_out = INFINITE) throw(SocketException);

private:
  void SetListen(int queueLen) throw(SocketException);
};

/**
  *   UDP socket class
  */
class EIB_STD_EXPORT UDPSocket : public CommunicatingSocket {
public:
  /**
   *   Construct a UDP socket
   *   @exception SocketException thrown if unable to create UDP socket
   */
  UDPSocket() throw(SocketException);

  /**
   *   Construct a UDP socket with the given local port
   *   @param localPort local port
   *   @exception SocketException thrown if unable to create UDP socket
   */
  UDPSocket(unsigned short localPort) throw(SocketException);

  /**
   *   Construct a UDP socket with the given local port and address
   *   @param localAddress local address
   *   @param localPort local port
   *   @exception SocketException thrown if unable to create UDP socket
   */
  UDPSocket(const CString &localAddress, unsigned short localPort)
      throw(SocketException);

  /**
   *   Unset foreign address and port
   *   @return true if disassociation is successful
   *   @exception SocketException thrown if unable to disconnect UDP socket
   */
  void Disconnect() throw(SocketException);

  /**
   *   Send the given buffer as a UDP datagram to the
   *   specified address/port
   *   @param buffer buffer to be written
   *   @param bufferLen number of bytes to write
   *   @param foreignAddress address (IP address or name) to send to
   *   @param foreignPort port number to send to
   *   @return true if send is successful
   *   @exception SocketException thrown if unable to send datagram
   */
  void SendTo(const void *buffer, int bufferLen, const CString &foreignAddress,
            int foreignPort) throw(SocketException);

  /**
   *   Send the given buffer as a UDP datagram to the
   *   specified address/port
   *   @param buffer buffer to be written
   *   @param bufferLen number of bytes to write
   *   @param foreignAddress address (IP address or name) to send to
   *   @param foreignPort port number to send to
   *   @param time_out max time to wait for socket to send data
   *   @return true if send is successful
   *   @exception SocketException thrown if unable to send datagram
   */
  void SendTo(const void *buffer, int bufferLen,
    const CString &foreignAddress, int foreignPort,int time_out);

  /**
   *   Read read up to bufferLen bytes data from this socket.  The given buffer
   *   is where the data will be placed
   *   @param buffer buffer to receive data
   *   @param bufferLen maximum number of bytes to receive
   *   @param sourceAddress address of datagram source
   *   @param sourcePort port of data source
   *   @return number of bytes received and -1 for error
   *   @exception SocketException thrown if unable to receive datagram
   */
  int RecvFrom(void *buffer, int bufferLen, CString &sourceAddress,
               int &sourcePort) throw(SocketException);

  /**
   *   Read read up to bufferLen bytes data from this socket.  The given buffer
   *   is where the data will be placed
   *   @param buffer buffer to receive data
   *   @param bufferLen maximum number of bytes to receive
   *   @param sourceAddress address of datagram source
   *   @param sourcePort port of data source
   *   @param time_out max time to wait for socket to receive data
   *   @return number of bytes received and -1 for error
   *   @exception SocketException thrown if unable to receive datagram
   */
  int RecvFrom(void *buffer, int bufferLen, CString &sourceAddress,int &sourcePort, int time_out) throw(SocketException);

  /**
   *   Set the multicast TTL
   *   @param multicastTTL multicast TTL
   *   @exception SocketException thrown if unable to set TTL
   */
  void SetMulticastTTL(unsigned char multicastTTL) throw(SocketException);

  /**
   *   Join the specified multicast group
   *   @param multicastGroup multicast group address to join
   *   @exception SocketException thrown if unable to join group
   */
  void JoinGroup(const CString& local_address,const CString& multicastGroup) throw(SocketException);

  /**
   *   Leave the specified multicast group
   *   @param multicastGroup multicast group address to leave
   *   @exception SocketException thrown if unable to leave group
   */
  void LeaveGroup(const CString& multicastGroup) throw(SocketException);

  /**
  *   Enable/Disable multicast packets on loopback interface
  *   @param val flag to Enable(true)/Disable(false)
  *   @exception SocketException thrown if unable to set the socket option
  */
  void SetMulticastLoopBack(bool val) throw(SocketException);

private:
  void SetBroadcast();
};

//#ifdef WIN32
//#pragma warning( default : 4290 )
//#endif

#endif
