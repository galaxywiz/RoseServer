#include "stdafx.h"
#include "Server.h"

Server::Server(ContentsProcess *contentsProcess)
{
	SLog(L"# Initialze network base");
	
	contentsProcess_ = contentsProcess;
	status_ = SERVER_STOP;

	xml_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initialize(&config);

	// db 초기화
	_db_manager;
}

Server::~Server()
{
	shutdownServer();

	status_ = SERVER_STOP;
	SAFE_DELETE(contentsProcess_);

	SLog(L"# End network base");
}

void Server::initialize(xml_t *config)
{
	//터미널 설정
	_terminal.run(this);

	//테스크 설정
	TaskManager::getInstance();

	//서버 설정
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Server");
	if (!root) {
		SLog(L"@ not exist server setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("IP");
	strcpy_s(ip_, elem->GetText());

	elem = root->FirstChildElement("Port");
	sscanf_s(elem->GetText(), "%d", &port_);

	elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &workerThreadCount_);

	status_ = SERVER_INITIALZE;
	SLog(L"* IO worker thread count : %d", workerThreadCount_);

	root = config->FirstChildElement("App");
	elem = root->FirstChildElement("Name");

	SLog(L"### %S start!!! ###", elem->GetText());
}

SERVER_STATUS &Server::status()
{
	return status_;
}

void Server::putPackage(Package *package)
{
	contentsProcess_->putPackage(package);
}