#include "session.h"
#include "../src_async/async.h"


#include <string>


Session::Session(boost::shared_ptr<boost::asio::io_service> io_service):m_socket(*io_service)
{

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

void Session::handle_read(const boost::system::error_code &ecode, size_t bytes)
{
    if(!ecode){

        //  waiting for some next async read
        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                                 boost::bind(&Session::handle_read, this,
                                             _1, boost::asio::placeholders::bytes_transferred)
                                 );
        // ~waiting for some next async read

    }else{
        if( (boost::asio::error::eof == ecode)||
                (boost::asio::error::connection_reset == ecode)){

        }

    }
}


// not used here, but need for some needs
void Session::handle_write(const boost::system::error_code &ecode)
{
    if(!ecode){
        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                                 boost::bind(&Session::handle_read, this,
                                             _1, boost::asio::placeholders::bytes_transferred)
                                 );
    }
}

