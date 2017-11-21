
#include "lepus/table.h"

namespace lepus {
    void Dictonary::SetValue(const String* key, const Value& value) {
        hash_map_[const_cast<String*>(key)] = value;
    }
    
    Value Dictonary::GetValue(const String* key) {
        HashMap::iterator iter = hash_map_.find(const_cast<String*>(key));
        if(iter != hash_map_.end())
            return iter->second;
        return Value();
    }
}
