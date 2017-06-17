#pragma once
#include "stdafx.h"
#include "Session.h"

#define SESSION_CAPACITY		(5000)

class SessionManager : public Singleton<SessionManager>
{
    typedef list<Session*>		SessionList;

    SessionList		            sessionList_;
	int						    sessionCount_;
	int						    maxConnection_;
    Lock                        lock_;

	oid_t						idSeed_;

    typedef std::function<void (SessionList *sessionList, wstr_t *arg)> cmdFunc;
	unordered_map<wstr_t, cmdFunc>   serverCommand_;

public:
	SessionManager();
	SessionManager(int maxConnection);
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