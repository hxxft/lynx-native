
#include "lepus/table.h"

namespace lepus {
    void Dictonary::SetValue(const std::string& key, const Value& value) {
        //hash_map_.insert(std::make_pair(key, value));
        hash_map_[key] = value;
    }
    
    Value Dictonary::GetValue(const std::string& key) {
        HashMap::iterator iter = hash_map_.find(key);
        if(iter != hash_map_.end())
            return iter->second;
        return Value();
    }
}
