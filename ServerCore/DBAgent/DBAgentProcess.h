#pragma once
#include "stdafx.h"

class DBAgentProcess : public ContentsProcess
{
public:
	DBAgentProcess();
private:
	void registSubPacketFunc();
	static void I_DB_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_REQ_LOAD_DATA(Session *session, Packet *rowPacket);
};