
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
        String* string = new String(str, strlen(str), this);
        NewString(string);
        return string;
    }
    
    String* StringPool::NewString(const std::string& str) {
        String* string = new String(str.c_str(), str.size(), this);
        NewString(string);
        return string;
    }
    
    String* StringPool::NewString(String*& string) {
        auto iter = string_set_.find(string);
        if(iter != string_set_.end()) {
            string->Release();
            string = *iter;
        }else{
            string_set_.insert(string);
        }
        string->AddRef();
        return string;
    }
    
    void StringPool::Earse(String* string) {
        string_set_.erase(string);
    }
}
