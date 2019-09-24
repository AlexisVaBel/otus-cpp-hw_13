#include "session.h"
#include "../src_async/async.h"

#include <thread>

#include <string>


Session::Session(boost::shared_ptr<boost::asio::io_service> ioService, std::shared_ptr<ThreadPool> threadPool, std::shared_ptr<BasicDB> db):m_ioService(ioService),
                 m_socket(*ioService), m_threadPool{threadPool}, m_db(db)
{

}

Session::~Session(){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    stop();
}

tcp::socket &Session::socket()
{
    return m_socket;
}

void Session::start()
{
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                             boost::bind(&Session::handle_read, this,
                                        _1, boost::asio::placeholders::bytes_transferred
                                         ));
}

void Session::stop()
{
    m_socket.close();
}

void Session::handle_read(const boost::system::error_code &ecode, size_t bytes)
{
    if(!ecode){

        m_ioService->post([self = shared_from_this()] () {self->on_command();});


        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
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


// not used here, but need for some needs
void Session::handle_write(const boost::system::error_code &ecode, size_t bytes)
{
    if(!ecode){
//        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
//                                 boost::bind(&Session::handle_read, this,
//                                             _1, boost::asio::placeholders::bytes_transferred)
//                                 );
    }
}

void Session::on_command()
{
    m_threadPool->add_task(
                [self = shared_from_this()] (){
        std::stringstream ss(self->m_data);
        std::vector<std::string> vct;
        std::string strElm;
        while( ss >> strElm)
            vct.push_back(strElm);
        size_t sz = 5;
        if(self->m_db->process_operations(vct)){
            self->m_data[0]='t';
            self->m_data[1]='r';
            self->m_data[2]='u';
            self->m_data[3]='e';
            self->m_data[4]='\n';
        }else{
            self->m_data[0]='f';
            self->m_data[1]='a';
            self->m_data[2]='l';
            self->m_data[3]='s';
            self->m_data[4]='e';
            self->m_data[5]='\n';
            sz = 6;
        }

        self->m_socket.async_write_some(boost::asio::buffer(self->m_data, sz),
                                 boost::bind(&Session::handle_write, self,
                                             _1, sz)
                                 );
    });
}

