#pragma once
#include "stdafx.h"

typedef enum
{
    DB_STOP,                // 정지
    DB_STANDBY,             // 쿼리 받을 준비
    DB_RUNNING,             // 쿼리 실행중
}DB_STATE;

class Database
{
protected:
    DB_STATE        state_;

public:
    Database()
    {
    }
    virtual ~Database()
    {
    }

    virtual bool connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password) = 0;
    virtual bool connect() = 0;
    virtual bool connected() = 0;
    virtual bool disconnect() = 0;

    virtual void run() = 0;
    DB_STATE &state() { return state_; }
};
