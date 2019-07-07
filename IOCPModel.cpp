#include "stdafx.h"
#include "IOCPModel.h"

#define WORKER_THREADS_PER_PROCESSOR 2
#define MAX_POST_ACCEPT              10
#define EXIT_CODE                    NULL

#define RELEASE(x)                   {if(x != NULL) {delete x; x = NULL;}}
#define RELEASE_HANDLE(x)            {if(x != NULL && x != INVALID_HANDLE_VALUE){CloseHandle(x); x = NULL;}}
#define RELEASE_SOCKET(x)            {if(x != INVALID_SOCKET) {closesocket(x); x = INVALID_SOCKET;}}

CIOCPModel::CIOCPModel(void) :
	m_nThreads(0),
	m_hShutdownEvent(NULL),
	m_hIOCompletionPort(NULL),
	m_phWorkerThreads(NULL),
	m_strIP(DEFAULT_IP),
	m_nPort(DEFAULT_PORT),
	m_lpfnAcceptEx(NULL),
	m_pListenContext(NULL)
{
}

CIOCPModel::~CIOCPModel(void)
{
	this->Stop();
}

DWORD WINAPI CIOCPModel::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	CIOCPModel* pIOCPModel = (CIOCPModel*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;

	pIOCPModel->_ShowMessage(_T("Start worker thread, ID: %d"), nThreadNo);

	OVERLAPPED          *pOverlapped = NULL;
	PER_SOCKET_CONTEXT  *pSocketContext = NULL;
	DWORD               dwBytesTransfered = 0;

	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITE
		);

		if (EXIT_CODE == (DWORD)pSocketContext)
		{
			break;
		}

		if (!bReturn)
		{
			DWORD dwErr = GetLastError();

			if (!pIOCPModel->HandleError(pSocketContext, dwErr))
			{
				break;
			}

			continue;
		}
		else
		{
			PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);

			if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType || SEND_POSTED == pIoContext->m_OpType))
			{
				pIOCPModel->_ShowMessage(_T("Client %s:%d is disconnected."), inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));

				//release resource
				pIOCPModel->_RemoveContext(pSocketContext);

				continue;
			}
			else
			{
				switch (pIoContext->m_OpType)
				{
					// Accept  
				case ACCEPT_POSTED:
				{

					//Using function _DoAccept to handle connect request
					pIOCPModel->_DoAccept(pSocketContext, pIoContext);


				}
				break;

				// RECV
				case RECV_POSTED:
				{
					//Using function _DoRecv to handle receive request
					pIOCPModel->_DoRecv(pSocketContext, pIoContext);
				}
				break;
				case SEND_POSTED:
				{
					//TODO
				}
				break;
				default:
					TRACE(_T("_WorkThread's pIoContext->m_OpType is abnormal.\n"));
					break;
				}
			}
		}
	}

	TRACE(_T("Worker thread num%d is quit.\n"), nThreadNo);

	//release thread parameter
	RELEASE(lpParam);

	return 0;
}

bool CIOCPModel::LoadSocketLib()
{
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (NO_ERROR != nResult)
	{
		this->_ShowMessage(_T("Initialize WinSock 2.2 failed！\n"));
		return false;
	}

	return true;
}

//start server
bool CIOCPModel::Start()
{
	// initialze
	InitializeCriticalSection(&m_csContextList);

	// create shut down event
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialze IOCP
	if (false == _InitializeIOCP())
	{
		this->_ShowMessage(_T("IOCP initialization failed！\n"));
		return false;
	}
	else
	{
		this->_ShowMessage(_T("\nIOCP initialization finished\n."));
	}

	// initialize Socket
	if (false == _InitializeListenSocket())
	{
		this->_ShowMessage(_T("Listen Socket initialization failed！\n"));
		this->_DeInitialize();
		return false;
	}
	else
	{
		this->_ShowMessage(_T("Listen Socket initialization finished."));
	}

	this->_ShowMessage(_T("System ready, waiting for connected....\n"));

	return true;
}


//close server
void CIOCPModel::Stop()
{
	if (m_pListenContext != NULL && m_pListenContext->m_Socket != INVALID_SOCKET)
	{
		//activate shut down event broadcast
		SetEvent(m_hShutdownEvent);

		for (int i = 0; i < m_nThreads; i++)
		{
			//broadcast all IOCP quit
			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
		}

		//waiting for client resource release
		WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

		//clear context list
		this->_ClearContextList();

		//release other resource
		this->_DeInitialize();

		this->_ShowMessage(_T("Stop listen\n"));
	}
}

//initialize IOCP
bool CIOCPModel::_InitializeIOCP()
{
	//create first IOCP
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == m_hIOCompletionPort)
	{
		this->_ShowMessage(_T("IOCP creation failed！ERROR num: %d!\n"), WSAGetLastError());
		return false;
	}

	//create thread according to the number of processor
	m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();

	//initialize handle for worker thread
	m_phWorkerThreads = new HANDLE[m_nThreads];

	//create worker thread based on calculation result
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;
		pThreadParams->pIOCPModel = this;
		pThreadParams->nThreadNo = i + 1;
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
	}

	TRACE("Already create %d _WorkerThread in total.\n", m_nThreads);

	return true;
}


//initialize socket
bool CIOCPModel::_InitializeListenSocket()
{
	//get function pointer
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	//server address in order to bind socket
	struct sockaddr_in ServerAddress;

	//generate socket information for listen
	m_pListenContext = new PER_SOCKET_CONTEXT;

	//overlapped IO
	m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_pListenContext->m_Socket)
	{
		this->_ShowMessage(_T("Socket initialization failed，Error num: %d.\n"), WSAGetLastError());
		return false;
	}
	else
	{
		TRACE("WSASocket() finished.\n");
	}

	//Bind Listen Socket to IOCP
	if (NULL == CreateIoCompletionPort((HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort, (DWORD)m_pListenContext, 0))
	{
		this->_ShowMessage(_T("Binding listen Socket to IOCP failed！Error num: %d/n"), WSAGetLastError());
		RELEASE_SOCKET(m_pListenContext->m_Socket);
		return false;
	}
	else
	{
		TRACE("Binding listen Socket to IOCP finished.\n");
	}

	//fulfill address information

	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	//binding random/specific IP
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	//USES_CONVERSION;
	//std::string s(W2A(m_strIP));
	//const char* ipstr = s.c_str();	
	//ServerAddress.sin_addr.s_addr = inet_addr(ipstr);         
	ServerAddress.sin_port = htons(m_nPort);

	//binding address and port
	if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
		this->_ShowMessage(_T("bind() function execution error.\n"));
		return false;
	}
	else
	{
		TRACE("bind() finished.\n");
	}

	//start listening
	if (SOCKET_ERROR == listen(m_pListenContext->m_Socket, SOMAXCONN))
	{
		this->_ShowMessage(_T("Listen() function execution error.\n"));
		return false;
	}
	else
	{
		TRACE("Listen() finished.\n");
	}

	//get function pointer of AcceptEx()
	DWORD dwBytes = 0;
	if (SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL))
	{
		this->_ShowMessage(_T("WSAIoctl fail to get AcceptEx function pointer. Error num: %d\n"), WSAGetLastError());
		this->_DeInitialize();
		return false;
	}

	//same to GuidGetAcceptExSockAddrs
	if (SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs),
		&m_lpfnGetAcceptExSockAddrs,
		sizeof(m_lpfnGetAcceptExSockAddrs),
		&dwBytes,
		NULL,
		NULL))
	{
		this->_ShowMessage(_T("WSAIoctl fail to get GuidGetAcceptExSockAddrs function pointer. Error num: %d\n"), WSAGetLastError());
		this->_DeInitialize();
		return false;
	}


	//prepare parameters for AcceptEx, then post AcceptEx I/O request
	for (int i = 0; i<MAX_POST_ACCEPT; i++)
	{
		//create a new IO_CONTEXT
		PER_IO_CONTEXT* pAcceptIoContext = m_pListenContext->GetNewIoContext();

		if (false == this->_PostAccept(pAcceptIoContext))
		{
			m_pListenContext->RemoveContext(pAcceptIoContext);
			return false;
		}
	}

	this->_ShowMessage(_T("%d AcceptEx requests posted"), MAX_POST_ACCEPT);

	return true;
}

//release all resource
void CIOCPModel::_DeInitialize()
{
	//delete critical section
	DeleteCriticalSection(&m_csContextList);

	//close handle
	RELEASE_HANDLE(m_hShutdownEvent);

	//release pointers of worker threads
	for (int i = 0; i<m_nThreads; i++)
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}

	RELEASE(m_phWorkerThreads);

	//close handle
	RELEASE_HANDLE(m_hIOCompletionPort);

	//close listen socket
	RELEASE(m_pListenContext);

	this->_ShowMessage(_T("Resource released.\n"));
}

//post accept request
bool CIOCPModel::_PostAccept(PER_IO_CONTEXT* pAcceptIoContext)
{
	ASSERT(INVALID_SOCKET != m_pListenContext->m_Socket);

	//prepare parameters
	DWORD dwBytes = 0;
	pAcceptIoContext->m_OpType = ACCEPT_POSTED;
	WSABUF *p_wbuf = &pAcceptIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;

	//prepare sockets for clients which will connected
	pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept)
	{
		_ShowMessage(_T("Socket creation failed! Error num: %d"), WSAGetLastError());
		return false;
	}

	//post AcceptEx
	if (FALSE == m_lpfnAcceptEx(m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, p_ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			_ShowMessage(_T("Post AcceptEx request failed, Error num: %d"), WSAGetLastError());
			return false;
		}
	}

	return true;
}

bool CIOCPModel::_DoAccept(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;
	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);

	//first, get address and other information of connected client
	this->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

	this->_ShowMessage(_T("Client %s:%d connected."), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port));
	this->_ShowMessage(_T("Client %s:%d information：%s."), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);


	//second, the information is from ListenSocket's context, which will be used to listen other new socket
	//we need to copy information into a new SocketContext for newly connected socket

	PER_SOCKET_CONTEXT* pNewSocketContext = new PER_SOCKET_CONTEXT;
	pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

	//binding this socket with IOCP
	if (false == this->_AssociateWithIOCP(pNewSocketContext))
	{
		RELEASE(pNewSocketContext);
		return false;
	}


	//third, create IOContext in it, in order to post first Recv data request on this socket
	PER_IO_CONTEXT* pNewIoContext = pNewSocketContext->GetNewIoContext();
	pNewIoContext->m_OpType = RECV_POSTED;
	pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;
	//if you want to save the contain of buffer
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	//after binding, we can post Recv request on this socket
	if (false == this->_PostRecv(pNewIoContext))
	{
		pNewSocketContext->RemoveContext(pNewIoContext);
		return false;
	}

	//fourth, if post succeeded, then add this vaild client information into ContextList
	this->_AddToContextList(pNewSocketContext);

	//fifth, after used, reset IOContext of Listen Socket, then ready to post new AcceptEx
	pIoContext->ResetBuffer();
	return this->_PostAccept(pIoContext);
}

//post Recv request
bool CIOCPModel::_PostRecv(PER_IO_CONTEXT* pIoContext)
{
	//initialize parameters
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = RECV_POSTED;

	//after initialization, post WSARecv request
	int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);

	//if return value error, and Error num is not Pending, then means this overlap request post failed
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		this->_ShowMessage(_T("Posting first WSARecv failed!"));
		return false;
	}
	return true;
}

//handle when receiving data achieved
bool CIOCPModel::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	//show latest data, then reset the mode and send next request
	SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;
	this->_ShowMessage(_T("Receive %s:%d information：%s"), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);

	//then post next WSARecv request
	return _PostRecv(pIoContext);
}



//binding socket with IOCP
bool CIOCPModel::_AssociateWithIOCP(PER_SOCKET_CONTEXT *pContext)
{
	//binding socket which is used to communicate with client with IOCP
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL == hTemp)
	{
		this->_ShowMessage(_T("Executing CreateIoCompletionPort() error.Error num：%d"), GetLastError());
		return false;
	}

	return true;
}

//add clients' information in to list
void CIOCPModel::_AddToContextList(PER_SOCKET_CONTEXT *pHandleData)
{
	EnterCriticalSection(&m_csContextList);

	m_arrayClientContext.Add(pHandleData);

	LeaveCriticalSection(&m_csContextList);
}

//remove specific Context
void CIOCPModel::_RemoveContext(PER_SOCKET_CONTEXT *pSocketContext)
{
	EnterCriticalSection(&m_csContextList);

	for (int i = 0; i<m_arrayClientContext.GetCount(); i++)
	{
		if (pSocketContext == m_arrayClientContext.GetAt(i))
		{
			RELEASE(pSocketContext);
			m_arrayClientContext.RemoveAt(i);
			break;
		}
	}

	LeaveCriticalSection(&m_csContextList);
}

//clear context list
void CIOCPModel::_ClearContextList()
{
	EnterCriticalSection(&m_csContextList);

	for (int i = 0; i<m_arrayClientContext.GetCount(); i++)
	{
		delete m_arrayClientContext.GetAt(i);
	}

	m_arrayClientContext.RemoveAll();

	LeaveCriticalSection(&m_csContextList);
}

//get local IP
CString CIOCPModel::GetLocalIP()
{
	//get local hostname
	char hostname[MAX_PATH] = { 0 };
	gethostname(hostname, MAX_PATH);
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if (lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	//get the first return IP in IP address list
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];

	//change IP address into string
	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);
	m_strIP = CString(inet_ntoa(inAddr));

	return m_strIP;
}

//get total number of processors
int CIOCPModel::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

void CIOCPModel::_ShowMessage(const CString szFormat, ...) const
{
	//TODO: print message on server main page
}

//check whether client's socket is disconnected
bool CIOCPModel::_IsSocketAlive(SOCKET s)
{
	int nByteSent = send(s, "", 0, 0);
	if (-1 == nByteSent) return false;
	return true;
}

//handle error on IOCP
bool CIOCPModel::HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr)
{
	//wait if is timeout  
	if (WAIT_TIMEOUT == dwErr)
	{
		//check whether client is still alive
		if (!_IsSocketAlive(pContext->m_Socket))
		{
			this->_ShowMessage(_T("Client disconnect abnormally"));
			this->_RemoveContext(pContext);
			return true;
		}
		else
		{
			this->_ShowMessage(_T("Timeout! Retrying..."));
			return true;
		}
	}

	// 可能是客户端异常退出了
	else if (ERROR_NETNAME_DELETED == dwErr)
	{
		this->_ShowMessage(_T("Client disconnect abnormally"));
		this->_RemoveContext(pContext);
		return true;
	}

	else
	{
		this->_ShowMessage(_T("IOCP operation error, thread quit. Error num：%d"), dwErr);
		return false;
	}
}