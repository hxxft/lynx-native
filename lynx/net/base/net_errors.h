#ifndef LYNX_NET_BASE_NET_ERRORS_H_
#define LYNX_NET_BASE_NET_ERRORS_H_

#include <string>

namespace net {
    // Error values are negative.
    enum Error {
        // No error.
        OK = 0,
        
#define NET_ERROR(label, value) ERR_ ## label = value,
#include "net/base/net_error_list.h"
#undef NET_ERROR
        
        // The value of the first certificate error code.
        ERR_CERT_BEGIN = ERR_CERT_COMMON_NAME_INVALID,
    };
    
    Error MapSystemError(int os_error);
    
    std::string ErrorToString(int error);
    
    std::string ErrorToShortString(int error);
}

#endif
