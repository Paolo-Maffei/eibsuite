//#include "Main.h"
#include <io.h>
#include <string.h>
#include "URLEncoding.h"
#include "CTime.h"
#include "EIBAddress.h"
#include "SharedMemory.h"

using namespace EibStack;

void main()
{
	CSharedMemory sh;
	if(sh.Open("MEMORY",5)){
		char buf[4];
		sh.Read(buf,3,INFINITE);
		sh.Read(buf,3,INFINITE);
	}
}