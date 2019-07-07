#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <string>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_LEN      8192
#define DEFAULT_PORT        8888
#define DEFAULT_IP          _T("127.0.0.1")

typedef enum _OPERATION_TYPE
{
	ACCEPT_POSTED,
	SEND_POSTED,
	RECV_POSTED,
	NULL_POSTED
}OPERATION_TYPE;

typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED      m_Overlapped;
	SOCKET          m_sockAccept;
	WSABUF          m_wsaBuf;
	char            m_szBuffer[MAX_BUFFER_LEN];
	OPERATION_TYPE  m_OpType;

	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;
		m_OpType = NULL_POSTED;
	}

	~_PER_IO_CONTEXT()
	{
		if (m_sockAccept != INVALID_SOCKET)
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}

	void ResetBuffer()
	{
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}
} PER_IO_CONTEXT, *PPER_IO_CONTEXT;

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET      m_Socket;
	SOCKADDR_IN m_ClientAddr;
	CArray<_PER_IO_CONTEXT*> m_arrayIoContext;

	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
	}

	~_PER_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		for (int i = 0; i<m_arrayIoContext.GetCount(); i++)
		{
			delete m_arrayIoContext.GetAt(i);
		}
		m_arrayIoContext.RemoveAll();
	}

	_PER_IO_CONTEXT* GetNewIoContext()
	{
		_PER_IO_CONTEXT* p = new _PER_IO_CONTEXT;
		m_arrayIoContext.Add(p);
		return p;
	}

	void RemoveContext(_PER_IO_CONTEXT* pContext)
	{
		//ASSERT(pContext != NULL);
		for (int i = 0; i<m_arrayIoContext.GetCount(); i++)
		{
			if (pContext == m_arrayIoContext.GetAt(i))
			{
				delete pContext;
				pContext = NULL;
				m_arrayIoContext.RemoveAt(i);
				break;
			}
		}
	}
} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

class CIOCPModel;
typedef struct _tagThreadParams_WORKER
{
	CIOCPModel* pIOCPModel;
	int         nThreadNo;
} THREADPARAMS_WORKER, *PTHREADPARAM_WORKER;

class CIOCPModel
{
public:
	CIOCPModel();
	~CIOCPModel();
public:
	//start server
	bool Start();

	//close server
	void Stop();

	//load socket library
	bool LoadSocketLib();

	//unload socket library
	void UnloadSocketLib() { WSACleanup(); }

	//get local IP address
	CString GetLocalIP();

	//set listen port
	void SetPort(const int& nPort) { m_nPort = nPort; }

protected:

	//initialize IOCP
	bool _InitializeIOCP();

	//initialize Socket
	bool _InitializeListenSocket();

	//release resources
	void _DeInitialize();

	//post accept request
	bool _PostAccept(PER_IO_CONTEXT* pAcceptIoContext);

	//post receive accept request
	bool _PostRecv(PER_IO_CONTEXT* pIoContext);

	//handle when client connect in
	bool _DoAccept(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);

	//handle when data achieved
	bool _DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);

	//add clients' information into list
	void _AddToContextList(PER_SOCKET_CONTEXT *pSocketContext);

	//remove clients' information in list
	void _RemoveContext(PER_SOCKET_CONTEXT *pSocketContext);

	//clean clients' information
	void _ClearContextList();

	//binding handle with IOCP
	bool _AssociateWithIOCP(PER_SOCKET_CONTEXT* pContext);

	//handle error on IOCP
	bool HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr);

	//ask for worker thread
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	//get number of processors
	int _GetNoOfProcessors();

	//check whether client's socket is disconnected
	bool _IsSocketAlive(SOCKET s);

	//show message
	void _ShowMessage(const CString szFormat, ...) const;

private:
	HANDLE                        m_hShutdownEvent;              //broadcast that system has shut down
	HANDLE                        m_hIOCompletionPort;           //handle of IOCP
	HANDLE*                       m_phWorkerThreads;            //pointer of worker thread
	int                           m_nThreads;                   //number of threads
	CString                       m_strIP;                      //IP address of server
	int                           m_nPort;                      //port listened by server
	CRITICAL_SECTION              m_csContextList;              //used in worker threads' synchronization
	CArray<PER_SOCKET_CONTEXT*>   m_arrayClientContext;         //context information of client's socket
	PER_SOCKET_CONTEXT*           m_pListenContext;             //context information of listen socket
	LPFN_ACCEPTEX                 m_lpfnAcceptEx;               //pointer of AcceptEx and GetAcceptExSockaddrs
	LPFN_GETACCEPTEXSOCKADDRS     m_lpfnGetAcceptExSockAddrs;
};
