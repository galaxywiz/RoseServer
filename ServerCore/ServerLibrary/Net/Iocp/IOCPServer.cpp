#include "stdafx.h"
#include "IOCPServer.h"
#include "IOCPSession.h"

IOCPServer::IOCPServer(ContentsProcess *contentsProcess)
:Server(contentsProcess)
{
}

IOCPServer::~IOCPServer()
{
	::closesocket(listenSocket_);
}

bool IOCPServer::createListenSocket()
{
    listenSocket_ = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket_ == INVALID_SOCKET) {
        SErrLog(L"! listenSocket fail");
        return false;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)port_);
	inet_pton(AF_INET, ip_, &(serverAddr.sin_addr));

	int reUseAddr = 1;
	setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, (char *)&reUseAddr, (int)sizeof(reUseAddr));

    int retval = ::bind(listenSocket_, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    if (retval == SOCKET_ERROR) {
        SErrLog(L"! bind fail");
        return false;
    }

    const int BACK_SOCKETS = 5;
    retval = ::listen(listenSocket_, BACK_SOCKETS);
    if (retval == SOCKET_ERROR) {
        SErrLog(L"! listen fail");
        return false;
    }

	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(serverAddr.sin_addr), ip.data(), ip.size());
	SLog(L"* server listen socket created, ip: %S, port: %d", ip.data(), port_);
    return true;
}

bool IOCPServer::run()
{
	if (MAX_IOCP_THREAD < workerThreadCount_) {
		SErrLog(L"! workerThread limit[%d], but config setting [%d]", MAX_IOCP_THREAD, workerThreadCount_);
		return false;
	}

	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, workerThreadCount_);
	if (iocp_ == nullptr) {
		return false;
	}
	this->createListenSocket();
	
	acceptThread_ = MAKE_THREAD(IOCPServer, acceptThread);
	for (int i = 0; i < workerThreadCount_; ++i) {
		workerThread_[i] = MAKE_THREAD(IOCPServer, workerThread);
	}
	this->status_ = SERVER_READY;

	while (!_shutdown) {
		wstring cmdLine;
		std::getline(std::wcin, cmdLine);

		SLog(L"Input was: %s", cmdLine.c_str());
		_session_manager.runCommand(cmdLine);
	}
	return true;
}

SOCKET IOCPServer::listenSocket()
{
    return listenSocket_;
}

HANDLE IOCPServer::iocp()
{
    return iocp_;
}

void IOCPServer::onAccept(SOCKET accepter, SOCKADDR_IN addrInfo)
{
    IOCPSession *session = new IOCPSession();
	if (session == nullptr) {
		SLog(L"! accept session create fail");
		return;
	}
	if (!session->onAccept(accepter, addrInfo)) {
		SAFE_DELETE(session);
		return;
	}
	if (!_session_manager.addSession(session)) {
		SAFE_DELETE(session);
		return;
	}
	session->ioData_[IO_READ].clear();

	HANDLE handle = CreateIoCompletionPort((HANDLE)accepter, this->iocp(), (ULONG_PTR)&(*session), NULL);
	if (!handle) {
		SAFE_DELETE(session);
		return;
	}
    
    SLog(L"* client accecpt from [%s]", session->clientAddress().c_str());
	session->recvStandBy();
}

DWORD WINAPI IOCPServer::acceptThread(LPVOID serverPtr)
{
	IOCPServer	*server = (IOCPServer *)serverPtr;
	
    while (!_shutdown) {
		SOCKET		acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAddr;
		static int addrLen = sizeof(recvAddr);
		acceptSocket = WSAAccept(server->listenSocket(), (struct sockaddr *)&recvAddr, &addrLen, NULL, 0);
		if (acceptSocket == SOCKET_ERROR) {
			if (!server->status() == SERVER_STOP) {
				SLog(L"! Accept fail");
				break;
			}
		}
		server->onAccept(acceptSocket, recvAddr);

		if (server->status() != SERVER_READY) {
			break;
		}
	}
	return 0;
}

DWORD WINAPI IOCPServer::workerThread(LPVOID serverPtr)
{
	IOCPServer *server = (IOCPServer *)serverPtr;

	while (!_shutdown) {
		IoData			*ioData = nullptr;
		IOCPSession	*session = nullptr;
		DWORD			transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->iocp(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED *)&ioData, INFINITE);
		if (!ret) {
			continue;
		}
		if (session == nullptr) {
			SLog(L"! socket data broken");
			return 0;
		}
		if (transferSize == 0) {
			SLog(L"* close by client[%d][%s]", session->id(), session->clientAddress().c_str());
			_session_manager.closeSession(session);
			continue;
		}

		switch (ioData->type()) {
		case IO_WRITE:
			session->onSend((size_t)transferSize);
			continue;

		case IO_READ:
			{
				Package *package = session->onRecv((size_t)transferSize);
				if (package != nullptr) {
					server->putPackage(package);
				}
			}
			continue;

		case IO_ERROR:
			SLog(L"* close by client error [%d][%s]", session->id(), session->clientAddress().c_str());
			_session_manager.closeSession(session);
			continue;
		}
	}
	return 0;
}
