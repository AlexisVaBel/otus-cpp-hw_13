#pragma once

#include <list>
#include <string>
#include <sstream>

struct CmdParser{

    static std::list<std::string> parse_cmd(char *chs, size_t bytes){
        std::list<std::string> vct;
        std::string item;
        std::stringstream ss(chs);
        while (std::getline(ss, item, ' ' )) {
            vct.push_back(item);
        }
        return vct;
    }
};
