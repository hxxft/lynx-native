
#ifndef LYNX_LEPUS_CONTEXT_H_
#define LYNX_LEPUS_CONTEXT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "lepus/value.h"
#include "lepus/string.h"

namespace lepus {
    class Global {
    public:
        Value* Get(int index) {
            if(index < global_content_.size()) {
                return &global_content_[index];
            }
            return nullptr;
        }
        
        int Search(String* name) {
            std::unordered_map<String*, int>::iterator iter = global_.find(name);
            if(iter !=  global_.end()) {
                return iter->second;
            }
            return -1;
        }
        
        int Add(String* name, Value value) {
            std::unordered_map<String*, int>::iterator iter = global_.find(name);
            if(iter !=  global_.end()) {
                return iter->second;
            }
            global_content_.push_back(value);
            global_.insert(std::make_pair(name, global_content_.size() - 1));
            return global_content_.size() - 1;
        }
    private:
        std::unordered_map<String*, int> global_;
        std::vector<Value> global_content_;
    };
    
    class Context {
    public:
        virtual ~Context(){}
        virtual void Initialize() = 0;
        virtual void Execute(const std::string& source) = 0;
        virtual Value Call(const std::string& name, const std::vector<Value>& args) = 0;
        virtual int GetParamsSize() = 0;
        virtual Value* GetParam(int index) = 0;
        void SetGlobalData(String* name, Value& value) {
            global_.Add(name, value);
        }
        Global& global() {
            return global_;
        }
        StringPool& string_pool(){
            return string_pool_;
        }
    protected:
        
        Global global_;
        StringPool string_pool_;
    };
}

#endif
