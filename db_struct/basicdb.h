#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <algorithm>
#include <tuple>
#include <vector>
#include <mutex>

#include "basictable.h"

const std::string DB_ERR_NOTABLE{"ERR no such Table"};


class BasicDB{


public:
    BasicDB(){
        m_tables["A"] = std::make_shared<BasicTable>();
        m_tables["B"] = std::make_shared<BasicTable>();
    };

    bool process_operations(const std::vector<std::string> &vct){
        auto fstCmd = vct.begin();
        if(fstCmd == vct.end()) return  false;
        if(fstCmd->compare("insert") == 0 || fstCmd->compare("INSERT") == 0) return insert(vct.at(1), std::stoi(vct.at(2)), vct.at(3));
        return false;
    }

    bool insert (const std::string strTable, size_t id, std::string str){
        std::lock_guard<std::mutex> write_mutex(m_mutex_main);
        auto table = m_tables.find(strTable);
        if(table == m_tables.end()) {
//            throw std::runtime_error(DB_ERR_NOTABLE);
            return false;
        }
        return table->second->insert_to_table(id, str);
    }

    std::vector<std::tuple<size_t,std::string,std::string>> intersect(){
        std::vector<int> v_intersection;
        std::vector<std::tuple<size_t,std::string,std::string>> tpl;

        std::lock_guard<std::mutex> read_mutex(m_mutex_main);
        auto ka = m_tables.at("A")->m_keys;
        auto kb = m_tables.at("B")->m_keys;

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


    std::vector<std::tuple<size_t,std::string,std::string>> simm_difference(){
        std::vector<int> v_simm_difference;
        std::vector<std::tuple<size_t,std::string,std::string>> tpl;

        std::lock_guard<std::mutex> read_mutex(m_mutex_main);


        auto ka = m_tables.at("A")->m_keys;
        auto kb = m_tables.at("B")->m_keys;

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

    void truncate(){
        std::lock_guard<std::mutex> write_mutex(m_mutex_main);
        m_tables.at("A")->truncate();
        m_tables.at("B")->truncate();
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
