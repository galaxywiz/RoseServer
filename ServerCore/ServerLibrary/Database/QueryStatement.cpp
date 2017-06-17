#pragma once
#include "stdafx.h"
#include "ADODatabase.h"
#include "QueryRecord.h"
#include "QueryStatement.h"

QueryStatement::QueryStatement()
{
	paramCount_ = 0;
	query_.clear();
}

QueryStatement::~QueryStatement()
{
}

void QueryStatement::setQuery(WCHAR *query, QUERY_TYPE type)
{
	query_ = query;
	type_ = type;
}

const WCHAR* QueryStatement::query()
{
	return query_.c_str();
}
QUERY_TYPE QueryStatement::type()
{
	return type_;
}

//------------------ 파라메터 추가 -----------------/
template<typename T>
void QueryStatement::addArg(WCHAR *fmt, T value)
{
	array<WCHAR, DB_PARAM_SIZE> buffer;
	snwprintf(buffer, fmt, value);
	if (paramCount_++) {
		query_ += L", ";
	}
	else {
		query_ += L" ";
	}
	query_ += buffer.data();
}

void QueryStatement::addParam(CHAR *value)
{
	this->addArg(L"'%S'", value);
}

void QueryStatement::addParam(WCHAR *value)
{
	this->addArg(L"'%s'", value);
}

void QueryStatement::addParam(INT32 value)
{
	this->addArg(L"'%d'", value);
}

void QueryStatement::addParam(UINT32 value)
{
	this->addArg(L"'%u'", value);
}

void QueryStatement::addParam(INT64 value)
{
	this->addArg(L"'%lld'", value);
}

void QueryStatement::addParam(UINT64 value)
{
	this->addArg(L"'%llu'", value);
}

void QueryStatement::addParam(FLOAT value)
{
	this->addArg(L"'%f'", value);
}

void QueryStatement::addParam(DOUBLE value)
{
	this->addArg(L"'%lf'", value);
}
