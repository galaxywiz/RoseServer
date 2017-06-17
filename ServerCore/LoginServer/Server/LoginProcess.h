#pragma once
#include "stdafx.h"

class LoginProcess : public ContentsProcess
{
public:
	LoginProcess();
private:
	void registSubPacketFunc();
	static void C_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_ANS_ID_PW(Session *session, Packet *rowPacket);
	static void I_LOGIN_NOTIFY_ID_LOADED(Session *session, Packet *rowPacket);
};