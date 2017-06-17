#include "stdafx.h"
#include "Assert.h"

void Assert(int condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo)
{
    if (condition) {
        return;
    }
    shutdownServer();

    wstr_t	msg;
    msg = L" Assert: ";
    msg += conditionStr;
    msg += L", file : ";
    msg += fileName;
    msg += L", line : ";

    WCHAR buf[16];
    _itow_s(lineNo, buf, 10);
    msg += buf;
    SLog(L"! error %s", msg.c_str());
    MiniDump::getInstance().execptionFilter(NULL);
}
