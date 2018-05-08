
#include "lepus/table.h"

namespace lepus {
    Dictonary::Dictonary() {}
    
    Dictonary::~Dictonary() {
        HashMap::iterator iter = hash_map_.begin();
        for(;iter != hash_map_.end(); ++iter) {
            iter->first->Release();
        }
    }
    
    void Dictonary::SetValue(const String* key, const Value& value) {
        const_cast<String*>(key)->AddRef();
        hash_map_[const_cast<String*>(key)] = value;
    }
    
    Value Dictonary::GetValue(const String* key) {
        HashMap::iterator iter = hash_map_.find(const_cast<String*>(key));
        if(iter != hash_map_.end())
            return iter->second;
        return Value();
    }
}
