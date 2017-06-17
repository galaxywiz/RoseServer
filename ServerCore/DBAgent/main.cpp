// DBAgent.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DBAgentProcess.h"

void serverProcess()
{
	shared_ptr<Server> server(new IOCPServer(new DBAgentProcess()));
	//std::shared_ptr<Server> server(new ServerASIO(2000));
	if (!server->run()) {
		SLog(L"! error: server start fail");
		return;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	DBManager::getInstance().run();

	shared_ptr<Thread> serverThread(new Thread(new thread_t(serverProcess), L"DBAgent"));
	return 0;
}

