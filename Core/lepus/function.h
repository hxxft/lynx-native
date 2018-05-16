#ifndef LYNX_LEPUS_FUNCTION_H_
#define LYNX_LEPUS_FUNCTION_H_

#include <vector>

#include "lepus/op_code.h"
#include "lepus/syntax_tree.h"
#include "lepus/upvalue.h"
#include "lepus/switch.h"

#include "base/ref_counted_ptr.h"

namespace lepus {
    class Value;
    class Function {
    public:
        Function() :op_codes_(),
                     const_values_(),
                     upvalues_(),
                     switches_(),
                     child_functions_(),
                     index_(0){
                         
                     }
        ~Function(){
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
        
        std::size_t AddConstNumber(double number);
        
        std::size_t AddConstString(String* string);
        
        std::size_t AddConstBoolean(bool boolean);
        
        std::size_t AddConstValue(const Value& v);
        
        std::size_t AddChildFunction(Function* function) {
            child_functions_.push_back(function);
            return child_functions_.size() - 1;
        }
        
        Function* GetChildFunction(long index) {
            return child_functions_[index];
        }
        
        
        
        Value* GetConstValue(std::size_t index) {
            return index < const_values_.size() ?
                &const_values_[index] : nullptr;
        }
        
        long SearchUpvalue(String* name) {
            for(long i = 0; i < upvalues_.size(); ++i) {
                if(upvalues_[i].name_ == name) {
                    return i;
                }
            }
            return -1;
        }
        
        long AddUpvalue(String* name, long register_index, bool in_parent_vars) {
            upvalues_.push_back(UpvalueInfo(name, register_index, in_parent_vars));
            return upvalues_.size() - 1;
        }
        
        UpvalueInfo* GetUpvalue(int index) {
            return &upvalues_[index];
        }
        
        std::size_t AddSwitch(SwitchInfo* info) {
            switches_.push_back(info);
            return switches_.size() - 1;
        }
        
        SwitchInfo* GetSwitch(long index) {
            return switches_[index];
        }
        
        std::size_t UpvaluesSize() {
            return upvalues_.size();
        }
        
        void set_index(size_t index) {
            index_ = index;
        }
        
        std::size_t index() {
            return index_;
        }
    private:
        std::vector<Instruction> op_codes_;
        
        std::vector<Value> const_values_;
        
        std::vector<UpvalueInfo> upvalues_;
        
        base::ScopedVector<SwitchInfo> switches_;
        
        base::ScopedVector<Function> child_functions_;
        
        std::size_t index_;
        
    };
    
    class Closure : public base::RefCountPtr<Closure>{
    public:
        Closure(Function* function)
        :function_(function){
            AddRef();
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
        
        Value* GetUpvalue(long index) {
            return upvalues_[index];
        }
        
    private:
        std::vector<Value*> upvalues_;
        Function* function_;
    };
}

#endif  // LYNX_LEPUS_FUNCTION_H_
