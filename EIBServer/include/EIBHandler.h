/*! \file EIBHandler.h
    \brief Defines CEIBHandler
    
	This is the header file for CEIBHandler

*/
#ifndef __EIB_HANDLER_HEADER__
#define __EIB_HANDLER_HEADER__

#include "JTC.h"
#include "Socket.h"
#include "Buffer.h"
#include "EIBNetIP.h"
#include "CemiFrame.h"
#include "IConnection.h"

using namespace std;
using namespace EibStack;

#define EIB_DELAY_TIME 200

typedef CBufferNoSync<KnxElementQueue, 100> CEIBIntefaceBuffer;

/*!
	\enum HANDLER_TYPE
	Defines the type of handler.
*/
enum HANDLER_TYPE
{
	INPUT_HANDLER,	//!<	An Input Handler handles data coming from the EIB system to the EIB Server
	OUTPUT_HANDLER	//!<	An Output Handler handles data coming from the EIB Server to the EIB system
};

/*! \class CEIBHandler
	\brief Reads and writes messages from and to the EIB system

	CEIBHandler uses an instance of CEIBInterface to read and write messages to and from the EIB system.\n
	An EIB Server uses:\n 
	One Input Handler that waits for it's _buffer to be filled with a message coming from the EIB system 
	and broadcasts this message to all the clients, using CClientsMgr.\n
	and\n
	One Output Handler that waits for it's _buffer to be filled with a message coming from any of the
	connected clients and sends this message to the EIB system, using CClientsMgr.\n
*/
class CEIBHandler : public JTCThread, public JTCMonitor
{
public:
	/*!
		constructor
		\param type Type of handler (INPUT_HANDLER or OUTPUT_HANDLER)
	*/
	CEIBHandler(HANDLER_TYPE type);
	/*!
		 destructor
	*/
	virtual ~CEIBHandler();
	
	/*!
		\fn void Write(const CCemi_L_Data_Frame& data, BlockingMode mode, JTCMonitor* optional_mon)
		\brief Writes a message to the EIB Device
		\param data Reference to the message to be written to the buffer.
		\param mode The sending mode Blocking/NonBlockging
		\param optional_mon an optional monitor used in case of Mode blocking. 
			   this monitor will be first aquired and released when ack/confirmation is received.
	*/
	void Write(const CCemi_L_Data_Frame& data, BlockingMode mode, JTCMonitor* optional_mon);
	/*!
		\fn virtual void Run(void* arg) 
		\brief Starting point for thread, calls either RunEIBReader or RunEIBWriter
		
	*/
	virtual void run();
	/*!
		Closes thread
		\fn void Close()
	*/
	void Close();

	void Suspend();
	void Resume();

private:
	/*!
		\fn void RunEIBReader(void* arg)
		\brief Output Handler main function

		Runs a loop, waits for it's _buffer to be filled with a message coming from the EIB system
		and broadcasts this message to all the clients, using CClientsMgr.
	*/
	void RunEIBReader();
	/*!
		\fn void RunEIBWriter(void* arg)
		\brief Input Handler main function

		Runs a loop, waits for it's _buffer to be filled with a message coming from any of the
		connected clients and sends this message to the EIB system, using CClientsMgr.

	*/
	void RunEIBWriter();

private:
	CEIBIntefaceBuffer _buffer;	//!	An instance of CBuffer, used for incoming/outgoing data
	HANDLER_TYPE _type;	//! Defines the type of this CEIBHandler (Input or Output)
	bool _stop;
	//Suspention section
	bool _pause;
	JTCMonitor _wait_mon;
};

#endif
