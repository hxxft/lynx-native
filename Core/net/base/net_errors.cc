#include "net/base/net_errors.h"

#include <string>

namespace net {
    std::string ErrorToString(int error) {
        return "net::" + ErrorToShortString(error);
    }
    
    std::string ErrorToShortString(int error) {
        if (error == 0)
            return "OK";
        
        const char* error_string;
        switch (error) {
#define NET_ERROR(label, value) \
case ERR_ ## label: \
error_string = # label; \
break;
#include "net/base/net_error_list.h"
#undef NET_ERROR
            default:
                error_string = "<unknown>";
        }
        return std::string("ERR_") + error_string;
    }
}
