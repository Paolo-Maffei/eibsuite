#ifndef __SHARED_MEMORY_HEADER__
#define __SHARED_MEMORY_HEADER__

#include "EibStdLib.h"

class EIB_STD_EXPORT CSharedMemory
{
private:
	class CWriteQueue
	{
	// This class is the queue, it contains a pointer to
	// a data block and a pointer to the next queue item.
	friend class CSharedMemory;
	
	private:
		CWriteQueue(int nDataSize);
		~CWriteQueue();
	
	private:
		void		*pData;
		CWriteQueue *pNext;
	};
public:
	enum
	{
		// Return values of the class-functions.
		MEM_ERROR_UNKNOWN		= -1,
		MEM_SUCCESS				= 0,
		MEM_ERROR_CLOSED		= 1,
		MEM_ERROR_TIMEOUT		= 2,
		MEM_ERROR_OTHERPARTY	= 3,
		MEM_ERROR_DATASIZE		= 4
	};

	CSharedMemory();
	virtual ~CSharedMemory();

	bool Open(char* sName, int nDataSize, int nTimeOut = INFINITE);
	void Close();
	int Write(void *pData, int nDataSize, DWORD dwTimeOut);
	int WriteToQueue(void *pData, int nDataSize);
	int Read(void *pData, int nDataSize, DWORD dwTimeOut);

private:
	void EmptyWriteQueue();
	static void QueueThread(void *pArg);
	
private:
	// We will use two shared memory pools to create a transparant memory 'pipe'.
	// One pool will be used as destination for one instance, and source for the other
	// instance, the other will be used the other way around.
	// The two mutexes will indicate which instance is already available.
	HANDLE			m_hSharedMemoryMutex[2];

	int				m_nInstanceID;
	int				m_nOtherInstanceID;

	HANDLE			m_hSharedMemory[2];

	BYTE*			m_pSharedMemory[2];

	int*			m_pSize;
	HANDLE			m_hClosed;			// This handle indicates wether this instance
										// is open or closed.
	HANDLE			m_hDataWrit[2];
	HANDLE			m_hDataRead[2];

	HANDLE			m_hSecondInstanceAvailable;

	// Queue stuff
	HANDLE			m_hQueueThread;
	HANDLE			m_hDataInQueue;
	CWriteQueue		*m_pFirst;
	HANDLE			m_hQueueMutex;
};

#endif // !defined(AFX_SHAREDMEMORY_H__86467BA6_5AFA_11D3_863D_00A0244A9CA7__INCLUDED_)
