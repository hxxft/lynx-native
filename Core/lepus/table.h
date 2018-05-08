#ifndef LYNX_LEPUS_TABLE_H_
#define LYNX_LEPUS_TABLE_H_

#include <unordered_map>
#include "lepus/value.h"
#include "lepus/lepus_string.h"
#include "base/scoped_ptr.h"
namespace lepus {
    class Dictonary {
    public:
        Dictonary();
        ~Dictonary();
        void SetValue(const String* key, const Value& value);
        Value GetValue(const String* key);
    private:
        typedef std::unordered_map<String*, Value> HashMap;
        HashMap hash_map_;
    };
}

#endif
