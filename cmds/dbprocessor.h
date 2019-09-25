#pragma once

#include "../db_struct/basicdb.h"

#include <list>
#include <string>

#include <memory>
#include <boost/asio.hpp>

static const std::string DB_CMD_ISNERT   ("INSERT");
static const std::string DB_CMD_TRUNCATE ("TRUNCATE");
static const std::string DB_CMD_INTERSECT("INTERSECTION");
static const std::string DB_CMD_SYM_DIFF ("SYMMETRIC_DIFFERENCE");

static const std::string DB_CMD_SHOW ("SHOW");


class DBProcessor: std::enable_shared_from_this<DBProcessor> {

    DBProcessor(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<BasicDB> db):m_socket(socket), m_db(db){

    }
public:
    ~DBProcessor(){}

    static auto make(std::shared_ptr<boost::asio::ip::tcp::socket> &socket, std::shared_ptr<BasicDB> db) {
        return std::shared_ptr<DBProcessor>(new DBProcessor(socket, db));
    }

    bool process_operations(const std::vector<std::string> &vct){
        if(vct.size() < 1) return false;

        std::string strCmd= vct.at(0);
        std::transform(strCmd.begin(), strCmd.end(), strCmd.begin(),
            [](unsigned char c){ return std::toupper(c); });


        if((strCmd.compare(DB_CMD_ISNERT) == 0 )) return insert_to_tbl(vct);


//        if((strCmd.compare(DB_CMD_INTERSECT) == 0 )) return intersect_tbls();
//        if((strCmd.compare(DB_CMD_SYM_DIFF) == 0 ))  return diff_tbls();

//        if((strCmd.compare(DB_CMD_SHOW) == 0 ))  return show_tbl(vct.at(1));

        return false;
    }


private:

    bool insert_to_tbl(const std::vector<std::string> &vct){
        if(vct.size() != 4) return false;
        try {
            m_db->insert(vct.at(1),std::stoi(vct.at(2)),vct.at(3) );
            push_back("OK");
        } catch (std::error_code &er) {
            push_back("BAD");
            return false;
        }
        return true;
    }

    bool truncate_tbl(const std::string &strTbl){
        return m_db->truncate(strTbl);
    }

    bool intersect_tbls(){
        return true;
    }

    bool diff_tbls(){
        return true;
    }

    bool show_tbl(const std::string &strName){
        auto rows = m_db->show_rows(strName);
        for(auto row: rows){
            std::cout << std::get<0>(row) << ", " << std::get<1>(row) << std::endl;
//            push_back(std::get<0>(row) + (std::get<1>(row)));
        }
        return true;
    }

    void send(std::string && responce){
        auto it = m_lstData.insert(m_lstData.cend(), std::move(responce));
        boost::asio::async_write(
                    *m_socket,
                    boost::asio::buffer(*it),
                    [self = shared_from_this(), it](boost::system::error_code, size_t){
                        self->m_lstData.erase(it);
                    });
    }

    void push_back(const std::string& resp) {
        send(resp + 'n');
    }

    void push_back(std::string&& resp) {
        resp += '\n';
        send(std::move(resp));
    }



    std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
    std::shared_ptr<BasicDB> m_db;

    std::list<std::string> m_lstData;
};
