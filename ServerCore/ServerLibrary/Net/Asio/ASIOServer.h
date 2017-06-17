#pragma once
#include "stdafx.h"
#ifdef USE_ASIO
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../Server.h"

using namespace std;
using namespace boost;
using namespace boost::asio;

class ASIOServer : public Server, public Singleton<ASIOServer>
{
	std::shared_ptr<io_service>		ptrService_;
	io_service						&ioService_;
	ip::tcp::acceptor				*acceptor_;

	strand							strand_;
	thread_group					group_;

	int seqNumber_;

public:
	ASIOServer(int maxConnection);
	virtual ~ASIOServer();

	bool		run();

private:
	void		startAccept();
	void		onAccept(Session *session, const system::error_code &error);
};
#endif //USE_ASIO