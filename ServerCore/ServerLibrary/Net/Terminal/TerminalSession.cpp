#pragma once
#include "stdafx.h"

bool TerminalSession::connectTo(char *ip, int port)
{
	socketData_.socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socketData_.socket_ == INVALID_SOCKET) {
		SLog(L"! terminal socket fail");
		return false;
	}
	ZeroMemory(&socketData_.addrInfo_, sizeof(socketData_.addrInfo_));
	socketData_.addrInfo_.sin_family = AF_INET;
	socketData_.addrInfo_.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(socketData_.addrInfo_.sin_addr));
	
	this->setSocketOpt();

	int ret = ::connect(socketData_.socket_, (sockaddr *)&socketData_.addrInfo_, sizeof(socketData_.addrInfo_));
	if (ret == SOCKET_ERROR) {
		SLog(L"! terminal socket connect fail");
		return false;
	}
	return true;
}

void TerminalSession::onSend(size_t transferSize)
{
	//Noting
}

void TerminalSession::sendPacket(Packet *packet)
{
	Stream stream;
	packet->encode(stream);

	packet_size_t offset = 0;
	array<char, SOCKET_BUF_SIZE> buffer;
	const size_t packetHeaderSize = sizeof(packet_size_t);

	//									 head size  + real data size
	packet_size_t packetLen[1] = { (packet_size_t)packetHeaderSize + (packet_size_t)stream.size(), };
	// insert packet len
	memcpy_s(buffer.data() + offset, buffer.max_size(), (void *)packetLen, packetHeaderSize);
	offset += packetHeaderSize;

	// packet obfuscation
	PacketObfuscation::getInstance().encodingHeader((Byte*)buffer.data(), packetHeaderSize);
	PacketObfuscation::getInstance().encodingData((Byte*)stream.data(), stream.size());

	// insert packet data
	memcpy_s(buffer.data() + offset, buffer.max_size(), stream.data(), packetLen[0]);
	offset += (packet_size_t)stream.size();

	::send(socketData_.socket_, buffer.data(), offset, 0);
}

Package* TerminalSession::onRecv(size_t transferSize)
{
	array<Byte, SOCKET_BUF_SIZE> rowData;
	int ret = ::recv(socketData_.socket_, (char *)rowData.data(), (int)rowData.size(), 0);
	if (ret <= 0) {
		return nullptr;
	}

	//패킷 길이 가지고 오기      
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };

	memcpy_s((void *)packetLen, sizeof(packetLen), (void *)rowData.data(), sizeof(packetLen));
	PacketObfuscation::getInstance().decodingHeader((Byte*)packetLen, sizeof(packetLen));

	while (ret < (int)packetLen[0]) {
		int len = ret;
		ret += ::recv(socketData_.socket_, (char *)rowData.data() + len, (int)rowData.size() - len, 0);
	}

	offset += sizeof(packetLen);
	PacketObfuscation::getInstance().decodingData((Byte*)rowData.data() + offset, packetLen[0] - offset);

	//서버간 패킷 처리
	Packet *packet = PacketAnalyzer::getInstance().analyzer((char *)rowData.data() + offset, packetLen[0]);
	if (packet == nullptr) {
		return nullptr;
	}

	Package *package = new Package(this, packet);
	return package;
}
