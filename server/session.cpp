#include "session.h"
#include "../src_async/async.h"

#include <thread>

#include <string>


Session::Session(boost::shared_ptr<boost::asio::io_service> ioService, std::shared_ptr<ThreadPool> threadPool, std::shared_ptr<BasicDB> db):m_ioService(ioService),
                 m_socket(std::make_shared<tcp::socket>(*ioService)), m_threadPool{threadPool}, m_db(db)
{
    m_procsDB = (DBProcessor::make(m_socket, m_db));
}

Session::~Session(){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    stop();
}

tcp::socket &Session::socket()
{
    return *m_socket;
}

void Session::start()
{
    m_socket->async_read_some(boost::asio::buffer(m_data, max_length),
                             boost::bind(&Session::handle_read, this,
                                        _1, boost::asio::placeholders::bytes_transferred
                                         ));
}

void Session::stop()
{
    m_socket->close();
}

void Session::handle_read(const boost::system::error_code &ecode, size_t bytes)
{
    if(!ecode){

        m_ioService->post([self = shared_from_this()] () {self->on_command();});


        m_socket->async_read_some(boost::asio::buffer(m_data, max_length),
                                 boost::bind(&Session::handle_read, this,
                                             _1, boost::asio::placeholders::bytes_transferred)
                                 );


    }else{
        if( (boost::asio::error::eof == ecode)||
                (boost::asio::error::connection_reset == ecode)){
            stop();
        }

    }
}




void Session::on_command()
{

    std::stringstream ss(m_data);
    std::vector<std::string> vct;
    std::string strElm;
    while( ss >> strElm)
        vct.push_back(strElm);

    m_procsDB->process_operations(vct);


//    m_threadPool->add_task(
//                [self = shared_from_this()] (){
//        std::stringstream ss(self->m_data);
//        std::vector<std::string> vct;
//        std::string strElm;
//        while( ss >> strElm)
//            vct.push_back(strElm);
//        self->m_procsDB = (DBProcessor::make(self->m_socket,self->m_db));
//        self->m_procsDB->process_operations(vct);
//    });

}

