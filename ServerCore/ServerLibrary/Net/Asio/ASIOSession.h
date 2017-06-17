#pragma once
#include "stdafx.h"
#ifdef USE_ASIO
#include <iostream>
#include <sstream> 

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

class Session;
class SessionManager;

using namespace std;
using namespace boost;
using namespace boost::asio;

class ASIOSession : public Session
{
	ip::tcp::socket socket_;
	str_t writeMessage_;
	std::array<char, SIZE_128> receiveBuffer_;

public:
	ASIOSession(io_service &ioService, SessionManager* clientManager);
	ip::tcp::socket &socket();

    void			sendPacket(char *packet, int len);
private:
	void			recvStandBy();

	void onSend(size_t transferSize)
	{
	}
	void onRecv(size_t transferSize)
	{
	}

	void onSend(size_t transferSize, const boost::system::error_code &error);
	void onRecv(size_t transferSize, const boost::system::error_code &error);

	template <typename T, typename Handler>
	void async_write(const T& t, Handler handler);
};
#endif //USE_ASIO