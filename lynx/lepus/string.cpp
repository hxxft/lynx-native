
#include "lepus/string.h"

namespace lepus{
    
    String::String(const char* str, std::size_t len, StringPool* string_pool) {
        string_pool_ = string_pool;
        length_ = len;
        str_ = new char[len + 1];
        memcpy(str_, str, len);
        str_[len] = 0;
        Hash(str_);
    }
    
    String::~String(){
        delete[] str_;
        string_pool_->Earse(this);
    }
    
    void String::Hash(const char *s)
    {
        hash_ = 5381;
        int c = 0;
        
        while ((c = *s++))
            hash_ = ((hash_ << 5) + hash_) + c;
    }
    
    
    String* StringPool::NewString(const char* str) {
        return NewString(std::string(str));
    }
    
    String* StringPool::NewString(const std::string& str) {
        auto iter = string_map_.find(str);
        if(iter != string_map_.end()) {
            return iter->second;
        }
        String* string = new String(str.c_str(), str.size(), this);
        string_map_.insert(std::make_pair(str, string));
        return string;
    }
    
    void StringPool::Earse(String* string) {
        string_map_.erase(string->c_str());
    }
}
