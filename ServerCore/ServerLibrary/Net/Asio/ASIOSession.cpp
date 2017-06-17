#include "stdafx.h"
#ifdef USE_ASIO
#include "../Session.h"
#include "../ASIOSession.h"
#include "../SessionManager.h"

ASIOSession::ASIOSession(io_service &ioService, SessionManager* clientManager)
: socket_(ioService)
, Session(clientManager)
{
}

ip::tcp::socket &ASIOSession::socket()
{
	return socket_;
}

void ASIOSession::recvStandBy()
{
	memset(&receiveBuffer_, '\0', sizeof(receiveBuffer_));
	socket_.async_read_some(buffer(receiveBuffer_),
		boost::bind(&ASIOSession::onRecv,
		this,
		boost::asio::placeholders::bytes_transferred,
		boost::asio::placeholders::error));
};

void ASIOSession::onSend(size_t transferSize, const boost::system::error_code &error)
{
	SLog(L"* send [%S] packet size [%d]", this->clientAddress().c_str(), transferSize);
}

void ASIOSession::onRecv(size_t transferSize, const boost::system::error_code &error)
{
	if (error) {
		if (error == error::eof) {
            SLog(L"* send close packet to client [%S]", this->clientAddress().c_str());
		}
		else {
			SLog(L"! error no : %d, msg: %S", error.value(), error.message().c_str());
		}
		return;
	}

	str_t recvMessage = receiveBuffer_.data();
    {
        //@@@임시 코드 여기에 패킷 매니져 삽입
        WCHAR wbuf[SIZE_1024];
        StrConvA2W((char *)recvMessage.c_str(), wbuf, SIZE_1024);
        WCHAR *len = wcschr(wbuf, '\r');
        *len = '\0';
		SLog(L"* [%d] recved(%d) : %s", GET_CURRENT_THREAD_ID(), transferSize, wbuf);
    }

	char message[SOCKET_BUF_SIZE] = { 0, };
	sprintf_s(message, sizeof(message) - 1, "Re : %S", recvMessage.c_str());
	writeMessage_ = message;
	this->sendPacket();

	this->recvStandBy();
}

void ASIOSession::sendPacket()
{
	boost::asio::async_write(socket_, buffer(writeMessage_),
		boost::bind(&ASIOSession::onSend,
		this,
		boost::asio::placeholders::bytes_transferred,
		boost::asio::placeholders::error));

	this->async_write(writeMessage_, this);
}

template <typename T, typename Handler>
void ASIOSession::async_write(const T& t, Handler handler)
{
#if 0
	// Serialize the data first so we know how large it is.
	std::ostringstream archive_stream;
	boost::archive::text_oarchive archive(archive_stream);
	archive << t;
	auto outbound_data_ = archive_stream.str();

	// Format the header.
	std::ostringstream header_stream;
	auto header_length = 4;
	header_stream << std::setw(header_length) << std::hex << outbound_data_.size();
	if (!header_stream || header_stream.str().size() != header_length)
	{
		// Something went wrong, inform the caller.
		boost::system::error_code error(error::invalid_argument);
	//	socket_.get_io_service().post(boost::bind(handler, error));
		return;
	}
	auto outbound_header_ = header_stream.str();

	// Write the serialized data to the socket. We use "gather-write" to send
	// both the header and the data in a single write operation.
	std::vector<const_buffer> buffers;
	buffers.push_back(buffer(outbound_header_));
	buffers.push_back(buffer(outbound_data_));
//	async_write(socket_, buffers, handler);
#endif
}
#endif //USE_ASIO