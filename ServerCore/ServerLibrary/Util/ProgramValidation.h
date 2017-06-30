#pragma once

// 만약 바이너리가 유출 되었을때를 대비한 안전장치 (서버 검증 장치)
#include "stdafx.h"
#include <locale>
#include <iostream>
#include <sstream>
#include <time.h>

class ProgramValidation
{
	void checkSMTP()
	{
		SOCKET smtpSocket;
		if (!connectSMTP(&smtpSocket)) {
			SLog(L"! The smtp server is not loaded.");
#ifndef _DEBUG
			exit(1);
#endif //_DEBUG
		}
	}

	class ProgramExpire
	{
		tick_t serverBirthTick_;

		void checkExpire()
		{
			tick_t expireTick = serverBirthTick_ + DAY_TO_TICK(30);
			tick_t now = CLOCK.systemTick();
			if (!isInRange(serverBirthTick_, now, expireTick)) {
				sendMail("serverAlert@server.com",
					"serverProgramer@server.com",
					"[EXPIRE] 서버 유효성 체크 실패",
					"IP주소, port 번호등...");
#ifndef _DEBUG
				exit(1);
#endif //_DEBUG
			}
		}

		void setBirthTick()
		{
			//문자열 시간을 tick_t화 하기
			//https://msdn.microsoft.com/en-us/library/ta5wcy3s.aspx
			locale loc;
			basic_stringstream<char> birthDate;
			ios_base::iostate st = 0;
			struct tm t;
			memset(&t, 0, sizeof(struct tm));

			birthDate << __DATE__;		//컴파일 날짜
			birthDate.imbue(loc);
			basic_istream<char>::_Iter i = use_facet
				<time_get <char> >
				(loc).get_date(basic_istream<char>::_Iter(birthDate.rdbuf()),
				basic_istream<char>::_Iter(0), birthDate, st, &t);

			if (st & ios_base::failbit) {
				cout << "time_get::get_time(" << birthDate.rdbuf()->str() << ") FAILED on char: " << *i << endl;
				ASSERT(FALSE);
			}
			array<WCHAR, SIZE_64> dataTime;
			snwprintf(dataTime, L"%4d-%2d-%2d %d:%d:%d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, 0, 0, 0);
			serverBirthTick_ = CLOCK.strToTick(dataTime.data());
		}

	public:
		ProgramExpire()
		{
			this->setBirthTick();
			this->checkExpire();
		}
	};
	

public:
	ProgramValidation()
	{
		ProgramExpire checkExpire;
		this->checkSMTP();
	}
};

static ProgramValidation programValidation;