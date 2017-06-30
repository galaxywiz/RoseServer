#pragma once
#include "stdafx.h"

class QI_DB_REQ_LOAD_DATA: public Query
{
public:
	oid_t clientId_;

	WCHAR *procedure()
	{
		return L"p_Character_Select";
	}

	QI_DB_REQ_LOAD_DATA() {
		statement_->setQuery(this->procedure(), QUERY_CALL_BACK);		// 실행할 쿼리 설정
	}

	~QI_DB_REQ_LOAD_DATA() {
		PK_I_DB_ANS_PARSE_DATA	iPacket;
		iPacket.clientId_ = (UInt64)clientId_;
		iPacket.result_ = FALSE;
		if (!record_.isEof()) {
			record_.moveFirst();
		}

		while (!record_.isEof()) {
			array<WCHAR, DB_PARAM_SIZE> buffer;

			if (record_.get("name", buffer.data())) {
				SLog(L"id : %s", buffer.data());
				array<CHAR, DB_PARAM_SIZE> nameBuf;
				StrConvW2A(buffer.data(), nameBuf.data(), nameBuf.size());
				iPacket.name_ = nameBuf.data();
				iPacket.result_ = TRUE;
			}
			record_.moveNext();
		}

		Terminal *terminal = _terminal.get(L"ChattingServer");
		terminal->sendPacket(&iPacket);
	}
};