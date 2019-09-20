#include "server.h"

#include "../src_async/async.h"

Server::Server(boost::shared_ptr<boost::asio::io_service> io_service, short port, size_t bulkSize):m_io_service(io_service),
    m_acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(tcp::v4(), port)),m_totalConnected(0),m_bulkSize(bulkSize)
{
    Session * new_session = new Session(m_io_service, async::connect(bulkSize));
    m_acceptor.async_accept(new_session->socket(),
                            boost::bind(&Server::handle_accept, this, new_session,
                                        _1));
}

void Server::handle_accept(Session *session, const boost::system::error_code &error)
{
    if(!error)
    {
        session->start();
        session = new Session(m_io_service, async::connect(m_bulkSize));
        m_acceptor.async_accept(session->socket(),
                                boost::bind(&Server::handle_accept, this , session,
                                            _1));        
    }else {
        delete session;
        return;
    }

}
