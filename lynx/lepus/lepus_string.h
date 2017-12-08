
#ifndef LYNX_LEPUS_STRING_H_
#define LYNX_LEPUS_STRING_H_

#include "base/ref_counted_ptr.h"
#include <unordered_set>
#include <unordered_map>

namespace lepus {
    class StringPool;
    class String : public base::RefCountPtr<String>{
    public:
        explicit String(const char* str, std::size_t len, StringPool* string_pool) ;
        virtual ~String();
        
        std::size_t hash() const{
            return hash_;
        }
        
        const char* c_str() {
            return str_;
        }
        
        int find(const String& other, int index) {
            char *str = strstr(str_+index, other.str_);
            if(str == nullptr)
                return -1;
            return str - str_;
        }
        
        friend bool operator == (const String& left, const String& right) {
            return memcmp(left.str_, right.str_, left.length_) == 0;
        }
    
    private:
        void Hash(const char *s);
        
        char* str_;
        std::size_t length_;
        std::size_t hash_;
        StringPool* string_pool_;
    };
    
    class StringPool {
    public:
        StringPool() : string_set_(), string_map_(){}
        ~StringPool() {
            
        }
        String* NewString(const char* str);
        String* NewString(const std::string& str);
    protected:
        friend class String;
        void Earse(String* string);
    private:
        struct Hash {
            std::size_t operator ()(const String* str) const {
                return str->hash();
            }
        };
        
        struct Equal {
            bool operator () (const String* left, const String* right) const {
                return left == right || *left == *right;
            }
        };
        std::unordered_set<String*, Hash, Equal> string_set_;
        std::unordered_map<std::string, String*> string_map_;
    };
}

#endif  // LYNX_LEPUS_STRING_H_
