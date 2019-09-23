#include "server.h"



Server::Server(boost::shared_ptr<boost::asio::io_service> io_service, short port):m_io_service(io_service),
    m_acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(tcp::v4(), port)),m_totalConnected(0)
{
    m_sesSet = std::make_shared<std::set<std::shared_ptr<Session>>>();
    std::shared_ptr<Session> new_session = std::make_shared<Session>(m_io_service);

    m_acceptor.async_accept(new_session->socket(),
                            boost::bind(&Server::handle_accept, this, new_session,
                                        _1));
}

void Server::handle_accept(std::shared_ptr<Session> session, const boost::system::error_code &error)
{
    if(!error)
    {
        session->start();
        m_sesSet->insert(session);

    }else {        
        std::cout << __PRETTY_FUNCTION__ << " " << error.message() << std::endl;
    }
    std::shared_ptr<Session> new_session = std::make_shared<Session>(m_io_service);
    m_acceptor.async_accept(new_session->socket(),
                            boost::bind(&Server::handle_accept, this , new_session,
                                        _1));

}
