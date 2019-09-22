#ifndef SERVER_H
#define SERVER_H

#include "session.h"


class Server
{
public:
    Server(boost::shared_ptr<boost::asio::io_service> io_service, short port);

    void handle_accept(Session *session, const boost::system::error_code &error);


private:
    boost::shared_ptr<boost::asio::io_service> m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    int m_totalConnected;


};

#endif // SERVER_H
