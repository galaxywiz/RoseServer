#pragma once
#include "stdafx.h"
#include "QueryStatement.h"
#include "QueryRecord.h"
#include "Query.h"
#include "ADODatabase.h"
#include "DBManager.h"

Query::Query()
{
	statement_ = new QueryStatement();
}
Query::~Query()
{
	record_.close();
	SAFE_DELETE(statement_);
}

void Query::setResult(recordPtr record)
{
	record_.setRecord(record);
}

QueryRecord& Query::result()
{
	return record_;
}

void Query::setStatement(QueryStatement *statement)
{
	statement_ = statement;
}

QueryStatement* Query::statement()
{
	return statement_;
}