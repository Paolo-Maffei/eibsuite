#ifndef __GENERAL_HEADER__
#define __GENERAL_HEADER__

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

#ifdef WIN32
#ifdef	EIBSTDLIB_EXPORTS
#define EIB_STD_EXPORT __declspec(dllexport)
#else
#define EIB_STD_EXPORT __declspec(dllimport)
#endif
#else
#define EIB_STD_EXPORT
#endif

#ifndef NULL
#define NULL 0x0
#endif

//////////////////////////////////// INCLUDE SECTION //////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <exception>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <list>
#include <time.h>
#include <vector>
#include <limits.h>
#include <map>
#include <fcntl.h>

#ifdef WIN32

#pragma warning( disable : 4290 )
#pragma warning( disable : 4251 )
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <process.h>
#include <string>

#else

#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

///////////////////////////////////////////////////////////////////////////////////////

#define ASSERT(cond) ASSERT_ERROR(cond,"")

#define ASSERT_ERROR(cond,msg) if(!(cond)){	throw CEIBException(GeneralError,"Assertion at file: %s: %d, %s",__FILE__,__LINE__,msg);}

#define START_TRY try{
#define END_TRY_START_CATCH(e) }catch(CEIBException& e){
#define END_TRY_START_CATCH_SOCKET(e) }catch(SocketException& e){
#define END_TRY_START_CATCH_JTC(e) }catch(const JTCException& e){
#define END_TRY_START_CATCH_GSM(e) }catch(GsmException& e){
#define END_TRY_START_CATCH_ANY }catch(...){
#define END_CATCH }

#endif
