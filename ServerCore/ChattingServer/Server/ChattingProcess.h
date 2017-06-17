#pragma once
#include "stdafx.h"

class ChattingProcess : public ContentsProcess
{
public:
	ChattingProcess();
private:
	void registSubPacketFunc();

	static void I_CHTTING_NOTIFY_ID(Session *session, Packet *rowPacket);
	static void I_DB_ANS_PARSE_DATA(Session *session, Packet *rowPacket);
	static void C_REQ_REGIST_CHATTING_NAME(Session *session, Packet *rowPacket);	
	static void C_REQ_CHATTING(Session *session, Packet *rowPacket);
	static void C_REQ_EXIT(Session *session, Packet *rowPacket);
};