// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BASE_PLATFORM_STRING_H_
#define BASE_PLATFORM_STRING_H_

namespace base {
    
    class PlatformString {
    public:
        virtual ~PlatformString() {}
        virtual const char* GetUTFChars() const = 0;
        virtual std::string ToString() const  = 0;
    };
    
}

#endif
