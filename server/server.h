#ifndef SERVER_H
#define SERVER_H

#include "session.h"
#include "../threader/threadpool.h"
#include "../db_struct/basicdb.h"

#include <set>
#include <memory>
#include <thread>

using sessions_set = std::set<std::shared_ptr<Session>>;

class Server: public std::enable_shared_from_this<Server>
{
public:
    Server(boost::shared_ptr<boost::asio::io_service> io_service, short port);
    ~Server();

    void handle_accept(std::shared_ptr<Session> session, const boost::system::error_code &error);


private:
    boost::shared_ptr<boost::asio::io_service> m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;


    std::shared_ptr<sessions_set> m_sesSet;

    std::shared_ptr<ThreadPool> m_threadPool;
    std::shared_ptr<BasicDB>    m_db;


    void stop();
};

#endif // SERVER_H
