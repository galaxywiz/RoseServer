#pragma once
#include "stdafx.h"
#include "packetHeader.h"
#include "packetClass.h"

#define _packet_factory		PacketFactory::getInstance()

class PacketFactory : public Singleton<PacketFactory>
{
public:
    Packet* getPacket(Int64 packetType) 
    {
        switch (packetType) {
            case E_C_REQ_EXIT:    return new PK_C_REQ_EXIT();
            case E_S_ANS_EXIT:    return new PK_S_ANS_EXIT();
            case E_I_NOTIFY_TERMINAL:    return new PK_I_NOTIFY_TERMINAL();
            case E_C_NOTIFY_HEARTBEAT:    return new PK_C_NOTIFY_HEARTBEAT();
            case E_C_REQ_ID_PW:    return new PK_C_REQ_ID_PW();
            case E_S_ANS_ID_PW_FAIL:    return new PK_S_ANS_ID_PW_FAIL();
            case E_S_ANS_ID_PW_SUCCESS:    return new PK_S_ANS_ID_PW_SUCCESS();
            case E_I_DB_REQ_ID_PW:    return new PK_I_DB_REQ_ID_PW();
            case E_I_DB_ANS_ID_PW:    return new PK_I_DB_ANS_ID_PW();
            case E_I_CHTTING_NOTIFY_ID:    return new PK_I_CHTTING_NOTIFY_ID();
            case E_I_DB_REQ_LOAD_DATA:    return new PK_I_DB_REQ_LOAD_DATA();
            case E_I_DB_ANS_PARSE_DATA:    return new PK_I_DB_ANS_PARSE_DATA();
            case E_I_LOGIN_NOTIFY_ID_LOADED:    return new PK_I_LOGIN_NOTIFY_ID_LOADED();
            case E_C_REQ_REGIST_CHATTING_NAME:    return new PK_C_REQ_REGIST_CHATTING_NAME();
            case E_C_REQ_CHATTING:    return new PK_C_REQ_CHATTING();
            case E_S_ANS_CHATTING:    return new PK_S_ANS_CHATTING();
        }
        return nullptr;
    }
};
