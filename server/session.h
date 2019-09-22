#ifndef SESSION_H
#define SESSION_H

#include <iostream>


#include <boost/bind.hpp>
#include <boost/asio.hpp>



using boost::asio::ip::tcp;


class Session
{
public:
    Session(boost::shared_ptr<boost::asio::io_service> io_service);
    tcp::socket &socket();

    void start();
    void handle_read(const boost::system::error_code &ecode, size_t bytes);
    void handle_write(const boost::system::error_code &ecode);
private:
    tcp::socket m_socket;
    enum { max_length = 1024};
    char m_data[max_length];
};

#endif // SESSION_H
