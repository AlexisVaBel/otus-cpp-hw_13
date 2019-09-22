#include "server.h"



Server::Server(boost::shared_ptr<boost::asio::io_service> io_service, short port):m_io_service(io_service),
    m_acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(tcp::v4(), port)),m_totalConnected(0)
{

//    m_acceptor.async_accept(new_session->socket(),
//                            boost::bind(&Server::handle_accept, this, new_session,
//                                        _1));
}

void Server::handle_accept(Session *session, const boost::system::error_code &error)
{
    if(!error)
    {
        session->start();

//        m_acceptor.async_accept(session->socket(),
//                                boost::bind(&Server::handle_accept, this , session,
//                                            _1));
    }else {
        delete session;
        return;
    }

}
