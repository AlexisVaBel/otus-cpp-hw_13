#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <algorithm>
#include <tuple>
#include <vector>
#include <mutex>
#include <algorithm>

#include "basictable.h"

const std::string DB_ERR_NOTABLE{"ERR no such Table"};


class BasicDB{


public:
    BasicDB(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        m_tables["A"] = std::make_shared<BasicTable>();
        m_tables["B"] = std::make_shared<BasicTable>();
    };


    bool insert (const std::string strTable, size_t id, std::string str){        
        std::lock_guard<std::mutex> write_mutex(m_mutex_main);
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        auto table = m_tables.find(strTable);
        if(table == m_tables.end()) {
            return false;
        }
        return table->second->insert_to_table(id, str);
    }


    std::vector<std::tuple<size_t,std::string,std::string>> intersect(std::string &strFst, std::string &strSnd){
        std::vector<int> v_intersection;
        std::vector<std::tuple<size_t,std::string,std::string>> tpl;

        std::lock_guard<std::mutex> read_mutex(m_mutex_main);
        auto ka = m_tables.at(strFst)->m_keys;
        auto kb = m_tables.at(strSnd)->m_keys;

        std::set_intersection(ka.begin(), ka.end(),
                              kb.begin(), kb.end(),
                              std::back_inserter(v_intersection));

        std::string str1("");
        std::string str2("");
        for(size_t idx: v_intersection){
            str1 = m_tables.at("A")->get_from_map(idx);
            str2 = m_tables.at("B")->get_from_map(idx);
            tpl.push_back(std::make_tuple(idx, str1, str2));
        }
        return  tpl;
    }

    std::vector<std::tuple<size_t,std::string,std::string>> intersect(){
        std::string str1 = "A";
        std::string str2 = "B";
        return  intersect(str1, str2);
    }

    std::vector<std::tuple<size_t,std::string>> show_rows(const std::string &strTable){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::vector<std::tuple<size_t,std::string>> tpl;
        if(m_tables.find(strTable) != m_tables.end()){
            auto tbl = m_tables.at(strTable);
            for(auto itm: tbl->m_mapRows){
                tpl.push_back(std::make_tuple(itm.first,itm.second));
            }
        }

        return tpl;
    }

    std::vector<std::tuple<size_t,std::string,std::string>> simm_difference(std::string &strFst, std::string &strSnd){
        std::vector<int> v_simm_difference;
        std::vector<std::tuple<size_t,std::string,std::string>> tpl;

        std::lock_guard<std::mutex> read_mutex(m_mutex_main);

        auto ka = m_tables.at(strFst)->m_keys;
        auto kb = m_tables.at(strSnd)->m_keys;

        std::set_symmetric_difference(ka.begin(), ka.end(),
                              kb.begin(), kb.end(),
                              std::back_inserter(v_simm_difference));

        std::string str1("");
        std::string str2("");
        for(size_t idx: v_simm_difference){
            str1 = m_tables.at("A")->get_from_map(idx);
            str2 = m_tables.at("B")->get_from_map(idx);
            tpl.push_back(std::make_tuple(idx, str1, str2));
        }
        return  tpl;
    }

    std::vector<std::tuple<size_t,std::string,std::string>> simm_difference(){
        std::string str1 = "A";
        std::string str2 = "B";
        return  simm_difference(str1, str2);
    }

    bool truncate(const std::string &strTable){
        std::lock_guard<std::mutex> write_mutex(m_mutex_main);
        if(m_tables.find(strTable) != m_tables.end()) m_tables.at(strTable)->truncate();
        else return false;
        return true;
    }


    friend std::ostream & operator << (std::ostream &out, const BasicDB &c){
        for(auto tableA: c.m_tables ){
            out << "Table " << tableA.first << std::endl;
            for(auto pr: tableA.second->m_mapRows){
                out << pr.first << " | " << pr.second << std::endl;
            }
        }
        return out;
    }
private:

    std::mutex  m_mutex_main;
    std::mutex  m_mutex_hash;

    std::map<std::string, std::shared_ptr<BasicTable>> m_tables;
};
