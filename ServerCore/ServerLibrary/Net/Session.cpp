#include "stdafx.h"
#include "Session.h"
#include "./Iocp/IOCPServer.h"

Session::Session()
{
	type_ = SESSION_TYPE_CLIENT;
	this->updateHeartBeat();
}

Session::~Session()
{
	this->onClose();
}

bool Session::setSocketOpt()
{
#ifdef linux
	int keepAlive = 1;
	int keepAliveIdle = 1;
	int keepAliveCnt = 3;
	int keepAliveInterval = 3;

	int ret;
	ret = ::setsockopt(socketData_.socket_, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPIDLE, &keepAliveIdle, sizeof(keepAliveIdle));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPCNT, &keepAliveCnt, sizeof(keepAliveCnt));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	ret = ::setsockopt(socketData_.socket_, SOL_TCP, SO_KEEPINTVL, &keepAliveInterval, sizeof(keepAliveInterval));
	if (ret == SOCKET_ERROR) {
		return false;
	}
#else
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 3000;        // Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;    // Resend if No-Reply

	DWORD dwBytes;
	if (WSAIoctl(socketData_.socket_, SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0) {
		return false;
	}
#endif
	return true;
}

bool Session::onAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	socketData_.socket_ = socket;
	int addrLen;
	getpeername(socketData_.socket_, (struct sockaddr *)&socketData_.addrInfo_, &addrLen);
	socketData_.addrInfo_ = addrInfo;
	if (!this->setSocketOpt()) {
		return false;
	}
	return true;
}

void Session::onClose(bool force)
{
	if (force) {
		_session_manager.forceCloseSession(this);
	}
	else {
		_session_manager.closeSession(this);
	}
}

SOCKET& Session::socket()
{
	return socketData_.socket_;
}

wstr_t Session::clientAddress()
{
	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(socketData_.addrInfo_.sin_addr), ip.data(), ip.size());

	array<WCHAR, SIZE_64> wip;
	StrConvA2W(ip.data(), wip.data(), wip.max_size());
	wstr_t stringData = wip.data();

	return stringData;
}

oid_t Session::id()
{
	return id_;
}

void Session::setId(oid_t id)
{
	id_ = id;
}

int8_t Session::type()
{
	return type_;
}

void Session::setType(int8_t type)
{
	type_ = type;
}

tick_t Session::heartBeat()
{
	return lastHeartBeat_;
}

void Session::updateHeartBeat()
{
	lastHeartBeat_ = CLOCK.systemTick();
}