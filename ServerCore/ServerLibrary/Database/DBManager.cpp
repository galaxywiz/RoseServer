#include "stdafx.h"

DBManager::DBManager()
{
	xml_t config;
	if (!loadConfig(&config)) {
		return;
	}

	this->initialize(&config);
}

void DBManager::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("DataBase");
	if (!root) {
		SLog(L"@ not exist database setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &workerCount_);

	array<WCHAR, SIZE_256> tmp;
	elem = root->FirstChildElement("ServerName");
	StrConvA2W((char *) elem->GetText(), tmp.data(), tmp.max_size());
	serverName_ = tmp.data();

	elem = root->FirstChildElement("DB");
	StrConvA2W((char *) elem->GetText(), tmp.data(), tmp.max_size());
	dbName_ = tmp.data();

	elem = root->FirstChildElement("Login");
	StrConvA2W((char *) elem->GetText(), tmp.data(), tmp.max_size());
	login_ = tmp.data();

	elem = root->FirstChildElement("Password");
	StrConvA2W((char *) elem->GetText(), tmp.data(), tmp.max_size());
	password_ = tmp.data();

	queryPool_ = new ThreadJobQueue<Query *>(L"DBQueueJob");

	for (int i = 0; i < workerCount_; ++i) {
		array<WCHAR, SIZE_128> patch = { 0, };
		ADODatabase *adodb = new ADODatabase();
		dbPool_.push_back(adodb);
	}
	this->run();
}

DBManager::~DBManager()
{
	SAFE_DELETE(queryPool_);

    for (auto db : dbPool_) {
        db->disconnect();
        SAFE_DELETE(db);
    }
}

size_t DBManager::runQueryCount()
{
	return queryPool_->size();
}

void DBManager::pushQuery(Query *query)
{
    queryPool_->push(query);
}

bool DBManager::popQuery(Query **query)
{
    return queryPool_->pop(*query);
}

void DBManager::run()
{
    for (auto db : dbPool_) {
        if (db->state() != DB_STOP) {
            continue;
        }

		if (!db->connect(serverName_.c_str(), dbName_.c_str(), login_.c_str(), password_.c_str())) {
			SErrLog(L"! db[%s] connection error", dbName_.c_str());
		}
        db->run();
    }
}