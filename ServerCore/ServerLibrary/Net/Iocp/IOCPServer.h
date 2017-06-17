#pragma once
#include "stdafx.h"
#include "../Server.h"

#define MAX_IOCP_THREAD		SIZE_64

class IOCPServer : public Server, public Singleton<IOCPServer>
{
    SOCKET					listenSocket_;
    HANDLE					iocp_;
	Thread					*acceptThread_;
	array<Thread *, SIZE_64> workerThread_;

private:
	bool					createListenSocket();

    static DWORD WINAPI		acceptThread(LPVOID serverPtr);
	static DWORD WINAPI		workerThread(LPVOID serverPtr);

public:
	IOCPServer(ContentsProcess *contentsProcess);
    virtual ~IOCPServer();

	bool					run();

    SOCKET					listenSocket();
    HANDLE					iocp();
	void					onAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
};
