#include "lepus/function.h"
#include "lepus/value.h"

namespace lepus {
    std::size_t Function::AddConstNumber(double number) {
        Value v;
        v.number_ = number;
        v.type_ = Value_Number;
        return AddConstValue(v);
    }
    
    std::size_t Function::AddConstString(String* string) {
        Value v;
        v.str_ = string;
        v.str_->AddRef();
        v.type_ = Value_String;
        return AddConstValue(v);
    }
    
    std::size_t Function::AddConstBoolean(bool boolean) {
        Value v;
        v.boolean_ = boolean;
        v.type_ = Value_Boolean;
        return AddConstValue(v);
    }
    
    std::size_t Function::AddConstValue(const Value& v) {
        for(size_t i = 0; i < const_values_.size(); i++) {
            if(const_values_[i] == v) {
                return i;
            }
        }
        const_values_.push_back(v);
        return const_values_.size() - 1;
    }
}
