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


//    std::cout << __PRETTY_FUNCTION__ << std::endl;
//    BasicDB db;
//    try {

//    db.insert("A",0,"lean");
//    db.insert("A",0,"mean");
//    db.insert("A",1,"sweater");
//    db.insert("A",2,"frank");
//    db.insert("A",3,"violation");
//    db.insert("A",4,"quality");
//    db.insert("A",5,"presicion");
//    db.insert("A",6,"desicion");

//    db.insert("B",3,"proposal");
//    db.insert("B",4,"example");
//    db.insert("B",5,"lake");
//    db.insert("B",6,"flour");
//    db.insert("B",7,"wonder");
//    db.insert("B",8,"selection");

//    auto t = db.simm_difference();

//    size_t idx;
//    std::string name1;
//    std::string name2;
//    for(auto el: t){
//        std::tie(idx, name1, name2) = el;
//        std::cout << " idx "<< idx << " " <<name1 << " " << name2 << std::endl;
//    }

//    } catch (std::runtime_error &er) {
//        std::cout << er.what() << std::endl;
//    }

//    std::cout << db ;
//    db.truncate();
//    std::cout << db ;


    return 0;
}
