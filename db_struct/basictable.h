#pragma once

#include <map>
#include <set>
#include <string>


const std::string TBL_ERR_DUPLICATE{"ERR duplicate"};
class BasicTable{
public:
    BasicTable(){

    }
    ~BasicTable(){

    }

    bool insert_to_table(size_t id, std::string str){
        auto t = m_mapRows.insert(std::pair<int, std::string>(id, str)).second;
        if (t) m_keys.insert(id);        
        return t;
    }

    void truncate(){
        m_mapRows = std::map<size_t, std::string>();
        m_mapHash = std::map<size_t, std::string>();
    }

    std::string get_from_map(const size_t &_idx){
        auto res = m_mapRows.find(_idx);
        if(res == m_mapRows.end()) return "---";
        return res->second;
    }

    std::map<size_t, std::string> m_mapRows;
    std::map<size_t, std::string> m_mapHash;
    std::set<size_t> m_keys;
};
