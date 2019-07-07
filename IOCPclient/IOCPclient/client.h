#pragma once

//shield deprecation warning
#pragma warning(disable: 4996)

//length of buffer(8*1024 bytes)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          8888                       //default port
#define DEFAULT_IP            _T("127.0.0.1")            //default IP address
#define DEFAULT_THREADS       100                        //default threads
#define DEFAULT_MESSAGE       _T("Hello!")               //default sending message

class CClient;

//thread parameters used to send data
typedef struct _tagThreadParams_WORKER
{
	CClient* pClient;                               //class pointer
	SOCKET   sock;                                  //Socket for every threads
	int      nThreadNo;                             //thread number
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER, *PTHREADPARAM_WORKER;

//thread to generate thread connection
typedef struct _tagThreadParams_CONNECTION
{
	CClient* pClient;                               //class pointer

} THREADPARAMS_CONNECTION, *PTHREADPARAM_CONNECTION;


class CClient
{
public:
	CClient(void);
	~CClient(void);

public:

	//load socket library
	bool LoadSocketLib();

	//unload socket library
	void UnloadSocketLib() { WSACleanup(); }

	//start client
	bool Start();

	//stop client
	void Stop();

	//get local IP address
	CString GetLocalIP();

	//set IP address for connection
	void SetIP(const CString& strIP) { m_strServerIP = strIP; }

	//set listen port
	void SetPort(const int& nPort) { m_nPort = nPort; }

	//set total number of threads
	void SetThreads(const int& n) { m_nThreads = n; }

	//set sending message
	void SetMessage(const CString& strMessage) { m_strMessage = strMessage; }

	//show message
	void ShowMessage(const CString strInfo, ...);

private:

	//establish connections
	bool EstablishConnections();

	//connect to server
	bool ConnetToServer(SOCKET *pSocket, CString strServer, int nPort);

	//thread used to generate connection
	static DWORD WINAPI _ConnectionThread(LPVOID lpParam);

	//thread used to sending message
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	//release resource
	void CleanUp();

private:

	CString   m_strServerIP;                                //server IP address
	CString   m_strLocalIP;                                 //local IP address
	CString   m_strMessage;                                 //message sent to server
	int       m_nPort;                                      //port be listened
	int       m_nThreads;                                   //total number of threads

	HANDLE    *m_phWorkerThreads;
	HANDLE    m_hConnectionThread;                          //thread socket to accept connection
	HANDLE    m_hShutdownEvent;                             //broadcast that system has shut down

	THREADPARAMS_WORKER      *m_pParamsWorker;              //thread parameters
};
