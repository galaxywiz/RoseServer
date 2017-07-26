#pragma once
#include "stdafx.h"

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
	for (auto itr : terminalPool_) {
		auto terminal = itr.second;
		SAFE_DELETE(terminal);
	}
}

void TerminalManager::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Terminal");
	xmlHandle_t terminalNode = TiXmlHandle(root);
	if (!root) {
		SLog(L"@ not exist terminal setting");
		return;
	}

	xmlNode_t *elem = terminalNode.FirstChildElement().Element();
	while (elem) {
		array<WCHAR, _MAX_PATH> terminalName;
		StrConvA2W((char *)elem->Value(), terminalName.data(), terminalName.max_size());

		Terminal *terminal = new Terminal(server_, terminalName.data());
		terminal->initialize(elem);
		this->put(terminalName.data(), terminal);

		elem = elem->NextSiblingElement();
	}

	SLog(L"### Terminal set ###");
}

void TerminalManager::put(wstr_t serverName, Terminal *terminal)
{
	terminalPool_.insert(make_pair(serverName, terminal));
}

Terminal* TerminalManager::get(wstr_t name)
{
	auto itr = terminalPool_.find(name);
	if (itr == terminalPool_.end()) {
		return nullptr;
	}
	return terminalPool_.at(name);
}

bool TerminalManager::isTerminal(const char *ip)
{
	for (auto terminal : terminalPool_) {
		if (!strcmp(terminal.second->ip(), ip)) {
			return true;
		}
	}
	return false;
}

void TerminalManager::run(Server *server)
{
	server_ = server;

	xml_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initialize(&config);
}