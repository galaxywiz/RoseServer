#pragma once
#include "stdafx.h"
#include "ADODatabase.h"

class QueryRecord
{
    recordPtr		record_;
	int				resultVal_;		//Äõ¸® °á°ú°ª
public:	
    QueryRecord();
    ~QueryRecord();

    void errorReport(_com_error &e);

    void operator = (QueryRecord &lvalue);
    void operator = (recordPtr &lvalue);
    recordPtr &resultRecord();

    bool        opened();
    void        close();
    void        setRecord(recordPtr record);
	bool		isEof();

	int			resultVal();
	void		setResultVal(int result);

	HRESULT		moveNext();
	HRESULT		movePrevious();
	HRESULT		moveFirst();
	HRESULT		moveLast();

	bool		get(char* fieldName, char* fieldValue);
	bool		get(char* fieldName, wchar_t* fieldValue);
	bool		get(char* fieldName, int32_t& fieldValue);
	bool		get(char* fieldName, int64_t& fieldValue);
	bool		get(char* fieldName, float& fieldValue);
	bool		get(char* fieldName, double& fieldValue);
	bool		get(char* fieldName, long& fieldValue);
};
