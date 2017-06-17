#pragma once
#include "stdafx.h"
#include "ADODatabase.h"

#include "QueryRecord.h"
#include "QueryStatement.h"
class Query
{
protected:
	QueryStatement		*statement_;
	QueryRecord			record_;
public:
	Query();
	virtual ~Query();
	
	void setResult(recordPtr record);
	QueryRecord &result();

	void setStatement(QueryStatement *statement);
	QueryStatement *statement();
};