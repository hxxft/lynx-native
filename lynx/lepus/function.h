#ifndef LYNX_LEPUS_FUNCTION_H_
#define LYNX_LEPUS_FUNCTION_H_

#include <vector>

#include "lepus/op_code.h"
#include "lepus/value.h"
#include "lepus/syntax_tree.h"
#include "lepus/upvalue.h"

#include "base/ref_counted_ptr.h"

namespace lepus {
    class Function {
    public:
        Function() :op_codes_(),
                     const_values_(),
                     upvalues_(),
                     child_functions_(),
                     index_(0){
                         
                     }
        
        std::size_t OpCodeSize() {
            return op_codes_.size();
        }
        
        const Instruction* GetOpCodes() const {
            return op_codes_.empty() ? nullptr : &op_codes_[0];
        }
        
        std::size_t AddInstruction(Instruction i) {
            op_codes_.push_back(i);
            return op_codes_.size() - 1;
        }
        
        Instruction* GetInstruction(std::size_t index)
        {
            return &op_codes_[index];
        }
        
        int AddConstNumber(double number) {
            Value v;
            v.number_ = number;
            v.type_ = Value_Number;
            return AddConstValue(v);
        }
        
        int AddChildFunction(Function* function) {
            child_functions_.push_back(function);
            return child_functions_.size() - 1;
        }
        
        Function* GetChildFunction(int index) {
            return child_functions_[index];
        }
        
        int AddConstValue(const Value& v) {
            for(size_t i = 0; i < const_values_.size(); i++) {
                if(const_values_[i] == v) {
                    return i;
                }
            }
            const_values_.push_back(v);
            return const_values_.size() - 1;
        }
        
        Value* GetConstValue(int index) {
            return index < const_values_.size() ?
                &const_values_[index] : nullptr;
        }
        
        int SearchUpvalue(String* name) {
            for(int i = 0; i < upvalues_.size(); ++i) {
                if(upvalues_[i].name_ == name) {
                    return i;
                }
            }
            return -1;
        }
        
        int AddUpvalue(String* name, int register_index, bool in_parent_vars) {
            upvalues_.push_back(UpvalueInfo(name, register_index, in_parent_vars));
            return upvalues_.size() - 1;
        }
        
        UpvalueInfo* GetUpvalue(int index) {
            return &upvalues_[index];
        }
        
        int UpvaluesSize() {
            return upvalues_.size();
        }
        
        void set_index(int index) {
            index_ = index;
        }
        
        int index() {
            return index_;
        }
    private:
        std::vector<Instruction> op_codes_;
        
        std::vector<Value> const_values_;
        
        std::vector<UpvalueInfo> upvalues_;
        
        std::vector<Function*> child_functions_;
        
        int index_;
        
    };
    
    class Closure : public base::RefCountPtr<Closure>{
    public:
        Closure(Function* function)
        :function_(function){
            
        }
        
        void set_function(Function* function) {
            function_ = function;
        }
        
        Function* function() {
            return function_;
        }
        
        void AddUpvalue(Value* value) {
            upvalues_.push_back(value);
        }
        
        Value* GetUpvalue(int index) {
            return upvalues_[index];
        }
        
    private:
        std::vector<Value*> upvalues_;
        Function* function_;
    };
}

#endif  // LYNX_LEPUS_FUNCTION_H_
