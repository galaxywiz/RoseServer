#pragma once
#include "stdafx.h"
#include "ContentsProcess.h"

ContentsProcess::ContentsProcess()
{
	xml_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initialize(&config);
}

ContentsProcess::~ContentsProcess()
{
	SAFE_DELETE(packageQueue_);

	for (auto thread : threadPool_) {
		SAFE_DELETE(thread);
	}
	runFuncTable_.clear();
}

void ContentsProcess::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Contents");
	if (!root) {
		SErrLog(L"* not exist process setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("ThreadCount");
	int processCount = 0;
	sscanf_s(elem->GetText(), "%d", &processCount);

	if (MAX_PACKET_THREAD_ < processCount) {
		SErrLog(L"! processThread limit[%d], but config setting [%d]", MAX_PACKET_THREAD_, processCount);
		return;
	}

	packageQueue_ = new ThreadJobQueue<Package *> (L"ContentsProcessQueue");
	for (int i = 0; i < processCount; ++i) {
		threadPool_[i] = MAKE_THREAD(ContentsProcess, process);
	}
	this->registDefaultPacketFunc();
}

void ContentsProcess::registDefaultPacketFunc()
{
	runFuncTable_.insert(make_pair(E_C_NOTIFY_HEARTBEAT, &ContentsProcess::Packet_HeartBeat));
	runFuncTable_.insert(make_pair(E_I_NOTIFY_TERMINAL, &ContentsProcess::Packet_Notify_Terminal));
	runFuncTable_.insert(make_pair(E_C_REQ_EXIT, &ContentsProcess::C_REQ_EXIT));
}

void ContentsProcess::putPackage(Package *package)
{
	packageQueue_->push(package);
}

void ContentsProcess::run(Package *package)
{
	PacketType type = package->packet_->type();
	auto itr = runFuncTable_.find(type);
	if (itr == runFuncTable_.end()) {
		SLog(L"! invaild packet runFunction. type[%d]", type);
		package->session_->onClose();
		return;
	}
	RunFunc runFunction = itr->second;
#ifdef _DEBUG
	SLog(L"*** [%d] packet run ***", type);
#endif //_DEBUG
	runFunction(package->session_, package->packet_);
}

void ContentsProcess::execute()
{
	Package *package = nullptr;
	if (packageQueue_->pop(package) == false) {
		return;
	}

	this->run(package);
	
	SAFE_DELETE(package);
}

void ContentsProcess::process()
{
	while (_shutdown == false) {
		this->execute();
		CONTEXT_SWITCH;
	}
}

//--------------------------------------------------------------//
// 기본 패킷 기능 구현
void ContentsProcess::Packet_HeartBeat(Session *session, Packet *rowPacket)
{
	if (session->type() != SESSION_TYPE_CLIENT) {
		return;
	}
	session->updateHeartBeat();
}

void ContentsProcess::Packet_Notify_Terminal(Session *session, Packet *rowPacket)
{
	session->setType(SESSION_TYPE_TERMINAL);
	SLog(L"* [%s] Terminal accepted.", session->clientAddress().c_str());
}

void ContentsProcess::C_REQ_EXIT(Session *session, Packet *rowPacket)
{
	//클라이언트 read thread 를 종료시켜 주기 위해 처리
	PK_C_REQ_EXIT *packet = (PK_C_REQ_EXIT *)rowPacket;
	PK_S_ANS_EXIT ansPacket;
	SLog(L"* recv exit packet by [%s]", session->clientAddress().c_str());
	session->sendPacket(&ansPacket);
}