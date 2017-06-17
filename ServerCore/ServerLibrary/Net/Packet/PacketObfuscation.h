#pragma once
#include "stdafx.h"

//--------------------------------------------------------//
// 난독화 최상위, 다른 난독화 알고리즘은 이 클래스를 상속 받을것
class Obfuscation
{
public:
	virtual void encodingHeader(Byte *packet, size_t packetLen) = 0;
	virtual void encodingData(Byte *packet, size_t packetLen) = 0;
	
	virtual void decodingHeader(Byte *packet, size_t packetLen) = 0;
	virtual void decodingData(Byte *packet, size_t packetLen) = 0;
};

//--------------------------------------------------------//
class XorObfuscation : public Obfuscation
{
	str_t	key_;
	int		keyLength_;

public:
	XorObfuscation();
private:
	void CalcXor(Byte *packet, int packetOffset, size_t packetLen);

	void encodingHeader(Byte *packet, size_t packetLen);
	void encodingData(Byte *packet, size_t packetLen);

	void decodingHeader(Byte *packet, size_t packetLen);
	void decodingData(Byte *packet, size_t packetLen);
};

//--------------------------------------------------------//
// 패킷 난독화용
class PacketObfuscation : public Singleton < PacketObfuscation >
{
	Obfuscation		*obfuscation_;
public:
	PacketObfuscation();
	~PacketObfuscation();

	void encodingHeader(Byte *packet, size_t packetLen);
	void encodingData(Byte *packet, size_t packetLen);

	void decodingHeader(Byte *packet, size_t packetLen);
	void decodingData(Byte *packet, size_t packetLen);
};
