#pragma once
#include <unordered_map>

#include "basictable.h"

struct BasicDB{

    bool insertA (int id, std::string str){
        return m_tableA.insert(std::pair<int, BasicTable>(id, {id,str})).second;
    }

    std::unordered_map<int, BasicTable> m_tableA;
    std::unordered_map<int, BasicTable> m_tableB;
};
