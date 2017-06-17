#include "packetHeader.h"
#include "Stream.h"

struct Packet {
    void encode(Stream &stream) {}
    void decode(Stream &stream) {}
};

struct PK_C_REQ_ID_PW : Packet
{
    int packetType() { return E_C_REQ_ID_PW; }
    std::string     id;
    std::string     password;

    void encode(Stream &stream) {
        stream << id;
        stream << password;
    }

    void decode(Stream &stream) {
        stream >> &id;
        stream >> &password;
    }
};

struct PK_S_REQ_ID_ID_PW : Packet
{
    int packetType() { return E_S_REQ_ID_ID_PW; }
    bool     result;

    void encode(Stream &stream) {
        stream << result;
    }

    void decode(Stream &stream) {
        stream >> &result;
    }
};

struct PK_C_NOTIFY_HEARTBEET : Packet
{
    int packetType() { return E_C_NOTIFY_HEARTBEET; }
};

struct PK_C_REQ_CHATTING : Packet
{
    int packetType() { return E_C_REQ_CHATTING; }
    std::string     text;

    void encode(Stream &stream) {
        stream << text;
    }

    void decode(Stream &stream) {
        stream >> &text;
    }
};

struct PK_S_REQ_CHATTING : Packet
{
    int packetType() { return E_S_REQ_CHATTING; }
    std::string     id;
    std::string     text;

    void encode(Stream &stream) {
        stream << id;
        stream << text;
    }

    void decode(Stream &stream) {
        stream >> &id;
        stream >> &text;
    }
};


