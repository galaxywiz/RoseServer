#include "packetHeader.h"
#include "Stream.h"

struct Packet {
    void encode(Stream &stream) {}
    void decode(Stream &stream) {}
};

struct PK_C_REQ_ID_PW : Packet
{
    int packetType() { return E_C_REQ_ID_PW; }
    string     id;
    string     password;

    void encode(Stream &stream) {
        stream.putString(id);
        stream.putString(password);
    }

    void decode(Stream &stream) {
        stream.getString(&id);
        stream.getString(&password);
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
    string     text;

    void encode(Stream &stream) {
        stream.putString(text);
    }

    void decode(Stream &stream) {
        stream.getString(&text);
    }
};

struct PK_S_REQ_CHATTING : Packet
{
    int packetType() { return E_S_REQ_CHATTING; }
    string     id;
    string     text;

    void encode(Stream &stream) {
        stream.putString(id);
        stream.putString(text);
    }

    void decode(Stream &stream) {
        stream.getString(&id);
        stream.getString(&text);
    }
};


