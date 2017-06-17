#include "stdafx.h"
#ifdef USE_ASIO
#include "../ASIOServer.h"
#include "../ASIOSession.h"

ASIOServer::ASIOServer(int maxConnection)
: Server(maxConnection)
, ptrService_(new io_service())
, ioService_(*ptrService_)
, strand_(ioService_)
{
}

ASIOServer::~ASIOServer()
{
}

bool ASIOServer::run()
{
	ip::tcp::endpoint endPoint(ip::address::from_string(ip_), port_);
	acceptor_ = new ip::tcp::acceptor(ioService_, endPoint);

	this->startAccept();

	for (int i = 0; i < workerThreadCount_; ++i) {
		group_.create_thread(boost::bind(&io_service::run, &ioService_));
	}
	return true;
}

void ASIOServer::startAccept()
{
	SLog(L"* startAccept...");

	ASIOSession *session = new ASIOSession(acceptor_->get_io_service(), this->clientManager());
	acceptor_->async_accept(session->socket(),
		boost::bind(&ASIOServer::onAccept,
		this,
		session,
		boost::asio::placeholders::error));

	if (!clientManager_->addSession(session)) {
		SLog(L"! maxConnection");
	}
}

void ASIOServer::onAccept(Session *session, const system::error_code &error)
{
	if (error) {
		return;
	}

	if (!this->clientManager()->addSession(session)) {
		return;
	}
	SLog(L"* client accepted");
	session->recvStandBy();
}
#endif //USE_ASIO