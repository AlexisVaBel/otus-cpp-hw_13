#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <set>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "../threader/threadpool.h"
#include "../db_struct/basicdb.h"
#include "../cmds/dbprocessor.h"




using boost::asio::ip::tcp;



class Session: public std::enable_shared_from_this<Session>
{
public:
    Session(boost::shared_ptr<boost::asio::io_service> ioService, std::shared_ptr<ThreadPool> threadPool, std::shared_ptr<BasicDB> db);
    ~Session();
    tcp::socket &socket();

    void start();
    void stop();
    void handle_read(const boost::system::error_code &ecode, size_t bytes);
    void handle_write(const boost::system::error_code &ecode, size_t bytes);
private:
    boost::shared_ptr<boost::asio::io_service> m_ioService;
    std::shared_ptr<tcp::socket> m_socket;
    enum { max_length = 1024};
    char m_data[max_length];    

    std::shared_ptr<ThreadPool> m_threadPool;
    std::shared_ptr<BasicDB>    m_db;
    std::shared_ptr<DBProcessor> m_procsDB;

    boost::asio::streambuf write_buffer;


    void on_command();

};

#endif // SESSION_H
