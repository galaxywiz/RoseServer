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
		
	// 원래는 가상함수로 호출해서 뒷처리를 시키는게 맞는데... 그러면 h, cpp 파일 둘다 만들어야 함.
	// 그냥 소멸자에 후처리를 하도록 처리
//protected:
//	virtual void doRespon() = 0;
};