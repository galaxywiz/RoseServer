#pragma once
#include "stdafx.h"

#define MAX_PACKET_THREAD_		SIZE_64
class ContentsProcess
{
private:
	array<Thread *, MAX_PACKET_THREAD_> threadPool_;
	ThreadJobQueue<Package *> *packageQueue_;

protected:
	typedef void(*RunFunc)(Session *session, Packet *rowPacket);
	unordered_map<PacketType, RunFunc> runFuncTable_;

private:
	void initialize(xml_t *config);
	void registDefaultPacketFunc();
	void run(Package *package);
	void execute();

	void process();
public:
	ContentsProcess();
	~ContentsProcess();
	
	void putPackage(Package *package);

	virtual void registSubPacketFunc() = 0;

	//--------------------------------------------------------------//
	// 기본 패킷 기능 구현
	static void Packet_HeartBeat(Session *session, Packet *rowPacket);
	static void Packet_Notify_Terminal(Session *session, Packet *rowPacket);
	static void C_REQ_EXIT(Session *session, Packet *rowPacket);
};
