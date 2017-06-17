#include "stdafx.h"
#include "Shutdown.h"

bool _shutdown = false;
void shutdownServer()
{
	SLog(L"### server shutdown!!! ###");

	//현재 접속중 모든 유저들 세션 종료
	//서버 데이터 정리 (랭킹이나 ...)
	// std::thread core();
	// core.join();
	//DB 커넥션 종료
}