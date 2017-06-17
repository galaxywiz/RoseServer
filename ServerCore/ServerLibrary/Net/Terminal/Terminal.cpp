#include "stdafx.h"
#include "Terminal.h"
#include "../Packet/PacketAnalyzer.h"

Terminal::Terminal(Server *server, wstr_t name)
{
	server_ = server;
	name_ = name;
}

Terminal::~Terminal()
{
	status_ = TERMINAL_STOP;
}

void Terminal::initialize(xmlNode_t *config)
{
	xmlNode_t *elem;

	elem = config->FirstChildElement("IP");
	strcpy_s(ip_, elem->GetText());

	elem = config->FirstChildElement("Port");
	sscanf_s(elem->GetText(), "%d", &port_);

	this->run();
}

TERMINAL_STATUS& Terminal::status()
{
	return status_;
}

void Terminal::sendPacket(Packet *packet)
{
	if (status_ == TERMINAL_READY) {
		session_.sendPacket(packet);
	}
}

const char *Terminal::ip()
{
	return ip_;
}

int Terminal::port()
{
	return port_;
}

void Terminal::connectProcess()
{
CONNECT_START:
	int tryCount = 0;
	while (_shutdown == false) {
		if (session_.connectTo(ip_, port_)) {
			break;
		}
		SLog(L"* try connect [%s] server[%S]:[%d]... [%d]", name_.c_str(), ip_, port_, tryCount++);
		Sleep(1000);        // 1초마다 연결 시도
	}
	status_ = TERMINAL_READY;

	// 자신이 터미널 세션임을 알려준다.
	PK_I_NOTIFY_TERMINAL terminalPacket;
	this->sendPacket(&terminalPacket);

	SLog(L"* [%s]terminal connect [%S]:[%d] ready", name_.c_str(), ip_, port_);
	while (_shutdown == false) {
		Package *package = session_.onRecv(0);

		if (package == nullptr) {
			SLog(L"! termnal [%s] disconnected !", name_.c_str());
			session_.onClose();
			goto CONNECT_START;
		}

		server_->putPackage(package);
	}
}

void Terminal::run()
{
	processThread_ = MAKE_THREAD(Terminal, connectProcess);
}
