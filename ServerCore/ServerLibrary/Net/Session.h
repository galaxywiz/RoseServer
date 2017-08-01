#pragma once
#include "stdafx.h"
// 유저 세션 구조체
// PC 온라인 게임 기준으로 작성하여서, 모바일 환경시 수정이 필요함.

struct SOCKET_DATA {
	SOCKET				socket_;
	SOCKADDR_IN			addrInfo_;
};

enum {
	SESSION_TYPE_TERMINAL,
	SESSION_TYPE_CLIENT,
};

class Session
{
protected:
	SOCKET_DATA		    socketData_;
	oid_t				id_;
	int8_t				type_;
	tick_t				lastHeartBeat_;
	bool				setSocketOpt();

public:
    Session();
    virtual ~Session();

	virtual bool		onAccept(SOCKET socket, SOCKADDR_IN addrInfo);

	virtual void		onSend(size_t transferSize) = 0;
	virtual void		sendPacket(Packet *packet) = 0;
	
	virtual Package*	onRecv(size_t transferSize) = 0;
	virtual void		recvStandBy() {};

    virtual void		onClose(bool force = false);

	SOCKET&				socket();
    wstr_t				clientAddress();

	oid_t				id();
	void				setId(oid_t id);

	int8_t				type();
	void				setType(int8_t type);

	tick_t				heartBeat();
	void				updateHeartBeat();
};
