#include "stdafx.h"
#include "ADODatabase.h"
#include "QueryStatement.h"
#include "DBManager.h"

ADODatabase::ADODatabase()
{
	::CoInitialize(NULL);
    state_ = DB_STOP;

	dbConnection_.CreateInstance(__uuidof(ADODB::Connection));
	if (dbConnection_ == NULL) {
		SErrLog(L"! Database init fail");
	}
    const int TIME_OUT = 30;
    this->setConnectTimeOut(TIME_OUT);
}

ADODatabase::~ADODatabase()
{
    this->disconnect();
    if (dbConnection_) {
        dbConnection_.Release();
    }
	SAFE_DELETE(thread_);
	::CoUninitialize();
}

HRESULT	ADODatabase::setConnectTimeOut(long second)
{
	if (!dbConnection_){
		return S_FALSE;
	}
	return dbConnection_->put_ConnectionTimeout(second);
}

void ADODatabase::comError(const WCHAR *actionName, _com_error &e)
{
    SLog(L"! [%s]DB [%s] fail [%s]", dbName_.c_str(), actionName, (WCHAR *)e.Description());
}

bool ADODatabase::connect()
{
	if (!dbConnection_) {
		return false;
	}

	try {
		HRESULT hr = dbConnection_->Open(connectionStr_.c_str(), _T(""), _T(""), NULL);
		if (SUCCEEDED(hr)) {
			dbConnection_->PutCursorLocation(ADODB::adUseClient);
			SLog(L"* [%s]DB connection success", dbName_.c_str());
			state_ = DB_STANDBY;
			return true;
		}
	}
	catch (_com_error &e) {
		this->comError(L"connction", e);
	}

	return false;
}

bool ADODatabase::connect(const WCHAR *provider, const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password)
{
	array<WCHAR, SIZE_128> buffer;
	snwprintf(buffer, L"Provider=%s;Server=%s;Database=%s;Uid=%s;Pwd=%s;", provider, serverName, dbName, id, password);
    connectionStr_ = buffer.data();
    SLog(L"* [%s]DB try connection provider = %s", dbName_.c_str(), provider);

	return this->connect();
}

bool ADODatabase::connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password)
{
    dbName_ = dbName;
	SLog(L"* connect try: %s, %s, %s", dbName, id, password);

	for (int index = 10; index < 20; ++index) {
		array<WCHAR, SIZE_64> mssqlName;
		snwprintf(mssqlName, L"SQLNCLI%d", index);
		if (this->connect(mssqlName.data(), serverName, dbName, id, password)) {
			SLog(L"* database %s : %s connect", mssqlName, dbName);
			return true;
		}
	}

	//mssql 2005, 2008로 접속시
	if (this->connect(L"SQLNCLI", serverName, dbName, id, password)) {
		SLog(L"* database SQLNCLI : %s connect",  dbName);
		return true;
	}
	return false;
}

bool ADODatabase::connected()
{
    return dbConnection_->State != ADODB::adStateClosed ? true : false;
}

bool ADODatabase::disconnect()
{
	if (!dbConnection_) {
		return false;
	}
    if (state_ == DB_STOP) {
        return true;
    }
	try {
        this->execute();

        if (!dbConnection_) {
            return true;
        }
		dbConnection_->Close();
        state_ = DB_STOP;

        connectionStr_.clear();
        dbName_.clear();
        SLog(L"* database close");
		return true;
	} catch (...) {
        SLog(L"! Database[%s] disconnect fail", dbName_.c_str());
	}
	return false;
}

void ADODatabase::execute()
{
	if (DBManager::getInstance().runQueryCount() == 0){
		return;
	}

	Query *query = nullptr;
	if (DBManager::getInstance().popQuery(&query) == false) {
		return;
	}
	QueryStatement *statement = query->statement();

	const WCHAR *sqlQuery = statement->query();
	try {
		state_ = DB_RUNNING;
		QueryRecord record;

		ADODB::_CommandPtr command;
		command.CreateInstance(__uuidof(ADODB::Command));
		command->ActiveConnection = dbConnection_;
		command->CommandType = ADODB::adCmdText;
		command->CommandText = sqlQuery;
		_variant_t resultVal;

		switch (statement->type()) {
		case QUERY_NOT_RETURN:
			record = command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adExecuteNoRecords);
			break;
		case QUERY_WAIT_RETURN:
			record = command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adExecuteRecord);
			break;
		case QUERY_CALL_BACK:
			record = command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adAsyncFetchNonBlocking);
			break;
		}

		if (record.isEof()) {
			int quertResultVal = atol((char*)((_bstr_t)resultVal));
			
			if (quertResultVal < 1) {
				SLog(L"* query : [%s] have error code [%d] ", sqlQuery, quertResultVal);
			}
			else {
				record.setResultVal(quertResultVal);
			}
		}
		
		query->result() = record;
		state_ = DB_STANDBY;

		SLog(L"* Run query [%s] result [%d]", sqlQuery, record.resultVal());
	}
	catch (_com_error &e) {
		this->comError(L"execute", e);
	}

	SAFE_DELETE(query);
}

void ADODatabase::process()
{
    while (_shutdown == false) {
		if (!this->connected()) {
			SLog(L"! db[%s] connection disconnect", dbName_.c_str());
			ASSERT(FALSE);
		}
		this->execute();

		CONTEXT_SWITCH;
    }
}

void ADODatabase::run()
{
	thread_ = MAKE_THREAD(ADODatabase, process);
}
