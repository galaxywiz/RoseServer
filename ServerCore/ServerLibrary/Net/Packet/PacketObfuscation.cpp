#pragma once
#include "stdafx.h"
#include "PacketObfuscation.h"

XorObfuscation::XorObfuscation()
{
	//암호 선택에 대해서는 조금 고민해 볼것.
	key_ = PACKET_MAKE_DATE;
	keyLength_ = (int)key_.length();
}

void XorObfuscation::CalcXor(Byte *packet, int packetOffset, size_t packetLen)
{
	int keyIdx = packetOffset % keyLength_;
	for (int packetIdx = 0; packetIdx < packetLen; ++packetIdx) {
		packet[packetIdx] ^= (Byte)key_[keyIdx++];

		if (keyIdx == keyLength_) {
			keyIdx = 0;
		}
	}
}

void XorObfuscation::encodingHeader(Byte *packet, size_t packetLen)
{
	this->CalcXor(packet, 0, packetLen);
}

void XorObfuscation::encodingData(Byte *packet, size_t packetLen)
{
	this->CalcXor(packet, sizeof(packet_size_t), packetLen);
}

void XorObfuscation::decodingHeader(Byte *packet, size_t packetLen)
{
	this->CalcXor(packet, 0, packetLen);
}

void XorObfuscation::decodingData(Byte *packet, size_t packetLen)
{
	this->CalcXor(packet, sizeof(packet_size_t), packetLen);
}
//--------------------------------------------------------//
PacketObfuscation::PacketObfuscation()
{
	//Todo : xor 말고 다른 알고리즘도 만들어서 config xml에서 선택하게 하기
	obfuscation_ = new XorObfuscation();
}

PacketObfuscation::~PacketObfuscation()
{
	SAFE_DELETE(obfuscation_);
}

void PacketObfuscation::encodingHeader(Byte *packet, size_t len)
{
	obfuscation_->encodingHeader(packet, len);
}

void PacketObfuscation::encodingData(Byte *packet, size_t len)
{
	obfuscation_->encodingData(packet, len);
}

void PacketObfuscation::decodingHeader(Byte *packet, size_t len)
{
	obfuscation_->decodingHeader(packet, len);
}

void PacketObfuscation::decodingData(Byte *packet, size_t len)
{
	obfuscation_->decodingData(packet, len);
}