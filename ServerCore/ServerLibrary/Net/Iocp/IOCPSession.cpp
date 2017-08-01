#include "stdafx.h"
#include "../Session.h"
#include "IOCPSession.h"
#include "../SessionManager.h"
#include "../Packet/PacketAnalyzer.h"

IoData::IoData()
{
	ZeroMemory(&overlapped_, sizeof(overlapped_));
	ioType_ = IO_ERROR;
	
	this->clear();
}

void IoData::clear()
{
	buffer_.fill(0);
	totalBytes_ = 0;
	currentBytes_ = 0;
}

bool IoData::needMoreIO(size_t transferSize)
{
	currentBytes_ += transferSize;
	if (currentBytes_ < totalBytes_) {
		return true;
	}
	return false;
}

int32_t IoData::setupTotalBytes()
{
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };
	if (totalBytes_ == 0) {
		memcpy_s((void *)packetLen, sizeof(packetLen), (void *)buffer_.data(), sizeof(packetLen));
		PacketObfuscation::getInstance().decodingHeader((Byte*)&packetLen, sizeof(packetLen));

		totalBytes_ = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}
size_t IoData::totalByte()
{
	return totalBytes_;
}

IO_OPERATION &IoData::type()
{
	return ioType_;
}

void IoData::setType(IO_OPERATION type)
{
	ioType_ = type;
}

char* IoData::data()
{
	return buffer_.data();
}

bool IoData::setData(Stream &stream)
{
	this->clear();

	if (buffer_.max_size() <= stream.size()) {
		SLog(L"! packet size too big [%d]byte", stream.size());
		return false;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t offset = 0;

	char *buf = buffer_.data();
	//									 head size  + real data size
	packet_size_t packetLen[1] = { (packet_size_t)packetHeaderSize + (packet_size_t)stream.size(), };
	// insert packet len
	memcpy_s(buf + offset, buffer_.max_size(), (void *)packetLen, packetHeaderSize);
	offset += packetHeaderSize;

	// packet obfuscation
	PacketObfuscation::getInstance().encodingHeader((Byte*)buf, packetHeaderSize);
	PacketObfuscation::getInstance().encodingData((Byte*)stream.data(), stream.size());

	// insert packet data
	memcpy_s(buf + offset, buffer_.max_size(), stream.data(), (int32_t)stream.size());
	offset += (packet_size_t)stream.size();

	totalBytes_ = offset;
	return true;
}

LPWSAOVERLAPPED IoData::overlapped()
{
	return &overlapped_;
}

WSABUF IoData::wsabuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = buffer_.data() + currentBytes_;
	wsaBuf.len = (ULONG)(totalBytes_ - currentBytes_);
	return wsaBuf;
}

//-----------------------------------------------------------------//
IOCPSession::IOCPSession()
: Session()
{
	this->initialize();
}

void IOCPSession::initialize()
{
	ZeroMemory(&socketData_, sizeof(SOCKET_DATA));
	ioData_[IO_READ].setType(IO_READ);
	ioData_[IO_WRITE].setType(IO_WRITE);
}

void IOCPSession::checkErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR
		&& (WSAGetLastError() != ERROR_IO_PENDING)) {
        SLog(L"! socket error: %d", WSAGetLastError());
	}
}

void IOCPSession::recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	DWORD errorCode = WSARecv(socketData_.socket_, &wsaBuf, 1, &recvBytes, &flags, ioData_[IO_READ].overlapped(), NULL);
	this->checkErrorIO(errorCode);
}

bool IOCPSession::isRecving(size_t transferSize)
{
	if (ioData_[IO_READ].needMoreIO(transferSize)) {
		this->recv(ioData_[IO_READ].wsabuf());
		return true;
	}
	return false;
}

void IOCPSession::recvStandBy()
{
	ioData_[IO_READ].clear();

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_READ].data();
	wsaBuf.len = SOCKET_BUF_SIZE;

	this->recv(wsaBuf);
}

void IOCPSession::send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(socketData_.socket_, &wsaBuf, 1, &sendBytes, flags, ioData_[IO_WRITE].overlapped(), NULL);
	this->checkErrorIO(errorCode);
}

void IOCPSession::onSend(size_t transferSize)
{
	if (ioData_[IO_WRITE].needMoreIO(transferSize)) {
		this->send(ioData_[IO_WRITE].wsabuf());
	}
}

void IOCPSession::sendPacket(Packet *packet)
{
	Stream stream;
	packet->encode(stream);
	if (!ioData_[IO_WRITE].setData(stream)) {
		return;
	}
	
	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_WRITE].data();
	wsaBuf.len = (ULONG) stream.size();

	this->send(wsaBuf);
    this->recvStandBy();
}

Package *IOCPSession::onRecv(size_t transferSize)
{
	packet_size_t offset = 0;
	offset += ioData_[IO_READ].setupTotalBytes();

	if (this->isRecving(transferSize)) {
		return nullptr;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t packetDataSize = (packet_size_t)(ioData_[IO_READ].totalByte() - packetHeaderSize);
	Byte *packetData = (Byte*) ioData_[IO_READ].data() + offset;

	PacketObfuscation::getInstance().decodingData(packetData, packetDataSize);
	Packet *packet = PacketAnalyzer::getInstance().analyzer((const char *)packetData, packetDataSize);
	if (packet == nullptr) {
		SLog(L"! invaild packet");
		this->onClose(true);
		return nullptr;
	}

	this->recvStandBy();

	Package *package = new Package(this, packet);
	return package;
}
