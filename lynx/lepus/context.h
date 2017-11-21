
#ifndef LYNX_LEPUS_CONTEXT_H_
#define LYNX_LEPUS_CONTEXT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "lepus/value.h"
#include "lepus/lepus_string.h"

namespace lepus {
    class Global {
    public:
        Global() : global_(), global_content_(){
        }
        
        ~Global() {
            std::unordered_map<String*, int>::iterator iter = global_.begin();
            for(;iter != global_.end(); ++iter) {
                iter->first->Release();
            }
        }
        
        Value* Get(std::size_t index) {
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
        
        std::size_t Add(String* name, Value value) {
            std::unordered_map<String*, int>::iterator iter = global_.find(name);
            if(iter !=  global_.end()) {
                return iter->second;
            }
            name->AddRef();
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
        Global* global() {
            return &global_;
        }
        StringPool* string_pool(){
            return &string_pool_;
        }
    protected:

        StringPool string_pool_;
        Global global_;
    };
}

#endif
