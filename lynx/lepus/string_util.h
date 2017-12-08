
#ifndef LYNX_LEPUS_STRING_UTIL_H_
#define LYNX_LEPUS_STRING_UTIL_H_

#include "lepus/lepus_string.h"


namespace lepus {
    void DeleteZero(std::string &str) {
        int pos = str.size() - 1 ;
        if(str.find('.') != std::string::npos){
            while(str.at(pos) == '0' && str.at(pos) != '.'){
                pos--;
            }
            str = str.substr(0, pos + 1);
        }
    }
}

#endif  // LYNX_LEPUS_STRING_API_H_

