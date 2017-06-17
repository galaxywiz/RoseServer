#pragma once
#include "stdafx.h"

// 터미널 쓰레드, 타 서버와 연결해서 데이터 송수신에 사용한다.
enum TERMINAL_STATUS {
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Server;
class Terminal
{
protected:
	Server					*server_;
	wstr_t					name_;
	TERMINAL_STATUS			status_;

	char					ip_[16];
	int						port_;

	TerminalSession			session_;

	Thread					*processThread_;

public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();
	TERMINAL_STATUS &status();

	void		initialize(xmlNode_t *config);
	void		sendPacket(Packet *packet);
	const char *ip();
	int			port();

private:
	void		connectProcess();
	void		run();
};
