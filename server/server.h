#ifndef SERVER_H
#define SERVER_H

#include "session.h"


class Server
{
public:
    Server(boost::shared_ptr<boost::asio::io_service> io_service, short port, size_t bulkSize);

    void handle_accept(Session *session, const boost::system::error_code &error);


private:
    boost::shared_ptr<boost::asio::io_service> m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    int m_totalConnected;

    size_t m_bulkSize;
};

#endif // SERVER_H
