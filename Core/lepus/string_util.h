
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
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif  // LYNX_LEPUS_STRING_API_H_

