#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>




#include "server/server.h"
#include "db_struct/basicdb.h"

int main(int argc, char ** argv){

    //    if(argc < 3) throw std::length_error("need port parameter and bulksize parameter");
    //    size_t portNum  = std::stoi(argv[1]);
    //    size_t bulkSize = std::stoi(argv[2]);

    boost::shared_ptr<boost::asio::io_service> m_io_service(
                new boost::asio::io_service);

    Server s(m_io_service, 30010); // param here
     m_io_service->run();



    return 0;
}
