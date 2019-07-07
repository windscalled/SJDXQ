#include "StdAfx.h"
#include "Client.h"

#include <winsock2.h>
#include <string>
#include <cstring>

#pragma comment(lib,"ws2_32.lib")

#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}

CClient::CClient(void) :
	m_strServerIP(DEFAULT_IP),
	m_strLocalIP(DEFAULT_IP),
	m_nThreads(DEFAULT_THREADS),
	m_nPort(DEFAULT_PORT),
	m_strMessage(DEFAULT_MESSAGE),
	m_phWorkerThreads(NULL),
	m_hConnectionThread(NULL),
	m_hShutdownEvent(NULL)

{
}

CClient::~CClient(void)
{
	this->Stop();
}

//thread used to generate connection
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_CONNECTION* pParams = (THREADPARAMS_CONNECTION*)lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	TRACE("_AccpetThread start£¬system listening...\n");

	pClient->EstablishConnections();

	TRACE(_T("_ConnectionThread thread shut down.\n"));

	RELEASE(pParams);

	return 0;
}

//thread used to sending message
DWORD WINAPI CClient::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER *pParams = (THREADPARAMS_WORKER *)lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	char szTemp[MAX_BUFFER_LEN];
	memset(szTemp, 0, sizeof(szTemp));
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv, 0, MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	//sending message to server just for test

	//TODO: sending function, including message, file, picture etc. 
	sprintf(szTemp, ("First message£º%s"), pParams->szBuffer);
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent)
	{
		TRACE("Error: fail to send one message, Error num£º%ld\n", WSAGetLastError());
		return 1;
	}
	TRACE("Sending message to server succeed: %s\n", szTemp);
	pClient->ShowMessage(_T("Sending message to server succeed: %s"), szTemp);

	Sleep(3000);

	return 0;
}

//establish connection
bool  CClient::EstablishConnections()
{
	DWORD nThreadID;

	m_phWorkerThreads = new HANDLE[m_nThreads];
	m_pParamsWorker = new THREADPARAMS_WORKER[m_nThreads];

	//according to the number of threads user set, generate every thread socket connect to server, and generate thread to send data 
	for (int i = 0; i<m_nThreads; i++)
	{
		//listen user's shut down event
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			TRACE(_T("Receive user's shut down command.\n"));
			return true;
		}

		//connect to server
		if (!this->ConnetToServer(&m_pParamsWorker[i].sock, m_strServerIP, m_nPort))
		{
			ShowMessage(_T("Fail to connect to server!"));
			CleanUp();
			return false;
		}

		m_pParamsWorker[i].nThreadNo = i + 1;
		sprintf(m_pParamsWorker[i].szBuffer, "No %d. thread sending data %s", i + 1, m_strMessage.GetString());

		Sleep(10);

		//if connect server succeeded, establish worker threads and sending data to server 
		m_pParamsWorker[i].pClient = this;
		m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
	}

	return true;
}

//connect to server
bool CClient::ConnetToServer(SOCKET *pSocket, CString strServer, int nPort)
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	//generate socket
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket)
	{
		TRACE("Error: Socket initialization failed, Error num£º%d\n", WSAGetLastError());
		return false;
	}

	//generate address
	USES_CONVERSION;
	std::string s(W2A(strServer));
	const char* ipstr = s.c_str();
	Server = gethostbyname(ipstr);
	if (Server == NULL)
	{
		closesocket(*pSocket);
		TRACE("Error: invalid IP address.\n");
		return false;
	}


	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr,
		(char *)Server->h_addr,
		Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	//start connect server
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress), sizeof(ServerAddress)))
	{
		closesocket(*pSocket);
		TRACE("Error: fail to connect to server\n");
		return false;
	}

	return true;
}

//initialize WinSock 2.2
bool CClient::LoadSocketLib()
{
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (NO_ERROR != nResult)
	{
		ShowMessage(_T("WinSock 2.2 initialization failed!\n"));
		return false; //Error
	}

	return true;
}

//start listen
bool CClient::Start()
{
	//establish shut down event broadcast
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//start connection thread
	DWORD nThreadID;
	THREADPARAMS_CONNECTION* pThreadParams = new THREADPARAMS_CONNECTION;
	pThreadParams->pClient = this;
	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

//stop listen
void CClient::Stop()
{
	if (m_hShutdownEvent == NULL) return;

	SetEvent(m_hShutdownEvent);
	//waiting for connection thread quit
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	//shut down all sockets
	for (int i = 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}

	//waiting for all worker threads quit
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	//release resource
	CleanUp();

	TRACE("test stop.\n");
}

//release resource
void CClient::CleanUp()
{
	if (m_hShutdownEvent == NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

//get local IP address
CString CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname, MAX_PATH);                //get hostname

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if (lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];       //get the first return IP in IP address list

	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);

	m_strLocalIP = CString(inet_ntoa(inAddr));      //change IP address into string

	return m_strLocalIP;
}

//show message
void CClient::ShowMessage(const CString strInfo, ...)
{
	//TODO: print message on client main page
}