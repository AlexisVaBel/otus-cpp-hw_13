#include "server.h"



Server::Server(boost::shared_ptr<boost::asio::io_service> io_service, short port):m_io_service(io_service),
    m_acceptor(*m_io_service, boost::asio::ip::tcp::endpoint(tcp::v4(), port))
{
    m_sesSet     = std::make_shared<sessions_set>();
    m_threadPool = std::make_shared<ThreadPool>(m_io_service) ;
    m_db         = std::make_shared<BasicDB>() ;

    auto backGroundThrs = std::thread::hardware_concurrency();

    for(decltype (backGroundThrs) i{0}; i < backGroundThrs ; ++i){
        m_threadPool->add_worker();
    }

    std::shared_ptr<Session> new_session = std::make_shared<Session>(m_io_service, m_threadPool, m_db);
    m_acceptor.async_accept(new_session->socket(),
                            boost::bind(&Server::handle_accept, this, new_session,
                                        _1));
}

Server::~Server()
{
    stop();
}

void Server::handle_accept(std::shared_ptr<Session> session, const boost::system::error_code &error)
{
    if(!error)
    {
        session->start();
        for(auto sess: *m_sesSet){
            if(!sess->socket().is_open()){
                m_sesSet->erase(sess);
            }

        }
        m_sesSet->insert(session);

    }else {        
        std::cout << __PRETTY_FUNCTION__ << " " << error.message() << std::endl;
    }
    std::shared_ptr<Session> new_session = std::make_shared<Session>(m_io_service, m_threadPool, m_db);
    m_acceptor.async_accept(new_session->socket(),
                            boost::bind(&Server::handle_accept, this , new_session,
                                        _1));

}

void Server::stop()
{
    for(const auto& ses: (*m_sesSet))
        ses->stop();
}
