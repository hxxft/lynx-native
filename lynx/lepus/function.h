#ifndef LYNX_LEPUS_FUNCTION_H_
#define LYNX_LEPUS_FUNCTION_H_

#include <vector>

#include "lepus/op_code.h"
#include "lepus/value.h"
#include "lepus/syntax_tree.h"

namespace lepus {
    class Function {
    public:
        std::size_t OpCodeSize() {
            return op_codes_.size();
        }
        
        const Instruction* GetOpCodes() const {
            return op_codes_.empty() ? nullptr : &op_codes_[0];
        }
        
        std::size_t AddInstruction(Instruction i) {
            op_codes_.push_back(i);
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
        
        void set_index(int index) {
            index_ = index;
        }
        
        int index() {
            return index_;
        }
    private:
        std::vector<Instruction> op_codes_;
        
        std::vector<Value> const_values_;
        
        std::vector<Function*> child_functions_;
        
        int index_;
        
    };
}

#endif  // LYNX_LEPUS_FUNCTION_H_
