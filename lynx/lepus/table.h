#ifndef LYNX_LEPUS_TABLE_H_
#define LYNX_LEPUS_TABLE_H_

#include <unordered_map>
#include "lepus/value.h"
#include "base/scoped_ptr.h"
namespace lepus {
    class Dictonary {
    public:
        void SetValue(const std::string& key, const Value& value);
        Value GetValue(const std::string& key);
    private:
        typedef std::unordered_map<std::string, Value> HashMap;
        HashMap hash_map_;
    };
}

#endif
