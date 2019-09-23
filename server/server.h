#ifndef SERVER_H
#define SERVER_H

#include "session.h"
#include <set>
#include <memory>
#include <thread>

class Server
{
public:
    Server(boost::shared_ptr<boost::asio::io_service> io_service, short port);
    ~Server();

    void handle_accept(std::shared_ptr<Session> session, const boost::system::error_code &error);


private:
    boost::shared_ptr<boost::asio::io_service> m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
    int m_totalConnected;

    std::shared_ptr<std::set<std::shared_ptr<Session>>> m_sesSet;

    std::shared_ptr<std::vector<std::thread>>    m_threads;
};

#endif // SERVER_H
