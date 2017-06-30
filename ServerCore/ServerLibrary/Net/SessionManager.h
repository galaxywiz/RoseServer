#pragma once
#include "stdafx.h"
#include "Session.h"
//http://duongame.tistory.com/36  다른 이야기이긴 하나 모바일 서버 만드는데 도움은 될듯
// 세션을 관리 하는 주체

#define SESSION_CAPACITY		(5000)
#define _session_manager		SessionManager::getInstance()

class SessionManager : public Singleton<SessionManager>
{
	// 세션의 빈번한 추가삭제를 위해 list로 구성
    typedef list<Session*>		SessionList;

    SessionList		            sessionList_;
	int									sessionCount_;
	int									maxConnection_;
    Lock								lock_;

	oid_t								sessionSeed_;			// 세션 메니져에서 관리하는 시드

	// 서버 수동 명령어
    typedef std::function<void (SessionList *sessionList, wstr_t *arg)> cmdFunc;
	unordered_map<wstr_t, cmdFunc>   serverCommand_;

public:
	SessionManager(int maxConnection = SESSION_CAPACITY);
	~SessionManager();
	oid_t				createSessionId();

	bool				addSession(Session *session);

	list<Session*>		&sessionList();
	bool				closeSession(Session *session);
	void				forceCloseSession(Session *session);

	Session				*session(oid_t id);

    void                runCommand(wstr_t cmd);
    void                commandFuncInitialize();
};