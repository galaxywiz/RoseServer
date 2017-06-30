#include "stdafx.h"
#include "QueryRecord.h"

QueryRecord::QueryRecord()
{
    record_.CreateInstance(__uuidof(ADODB::Recordset));
}

QueryRecord::~QueryRecord()
{
    if (record_ == nullptr) {
        return;
    }
    record_.Release();
    record_ = nullptr;
}

void QueryRecord::errorReport(_com_error &e) {
    SLog(L"* Query error = %S", e.Description());
}

recordPtr &QueryRecord::resultRecord()
{
    return record_;
}

void QueryRecord::operator = (QueryRecord &lvalue)
{
	record_ = lvalue.resultRecord();
}

void QueryRecord::operator = (recordPtr &lvalue)
{
    record_ = lvalue;
}

void QueryRecord::setRecord(recordPtr record)
{
    record_ = record;
}

bool QueryRecord::opened()
{
    return record_->State == ADODB::adStateOpen ? true : false;
}

void QueryRecord::close()
{
    try{
        if (record_ != nullptr && this->opened())
            record_->Close();
    }
    catch (_com_error &e){
        this->errorReport(e);
    }
}

int QueryRecord::resultVal()
{
	return resultVal_;
}

void QueryRecord::setResultVal(int result)
{
	resultVal_ = result;
}

bool QueryRecord::isEof()
{
	if (record_ == nullptr) {
		return true;
	}
	try {
		return record_->EndOfFile ? true : false;
	}
	catch (_com_error &e) {
		this->errorReport(e);
	}
	return false;
}

HRESULT QueryRecord::moveNext()
{
	try {
		return record_->MoveNext();
	}
	catch (_com_error &e) {
		this->errorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::movePrevious()
{
	try {
		return record_->MovePrevious();
	}
	catch (_com_error &e) {
		this->errorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::moveFirst()
{
	try {
		return record_->MoveFirst();
	}
	catch (_com_error &e) {
		this->errorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::moveLast()
{
	try {
		return record_->MoveLast();
	}
	catch (_com_error &e) {
		this->errorReport(e);
	}
	return S_FALSE;
}

bool QueryRecord::get(char* fieldName, char* fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		sprintf_s(fieldValue, DB_PARAM_SIZE, "%s", (LPCSTR)((_bstr_t)vtValue.bstrVal));
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, wchar_t* fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		swprintf_s(fieldValue, DB_PARAM_SIZE, L"%s", (LPWSTR)((_bstr_t)vtValue.bstrVal));
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, int32_t& fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.intVal;
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, int64_t& fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.intVal;
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, float& fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.fltVal;
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, double& fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.dblVal;
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::get(char* fieldName, long& fieldValue)
{
	try {
		_variant_t  vtValue;
		vtValue = record_->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.lVal;
		return true;
	}
	catch (_com_error &e) {
		this->errorReport(e);
		SLog(L"! error query field : %S", fieldName);
	}
	return false;
}