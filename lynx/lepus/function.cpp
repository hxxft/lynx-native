#include "lepus/function.h"
#include "lepus/value.h"

namespace lepus {
    int Function::AddConstNumber(double number) {
        Value v;
        v.number_ = number;
        v.type_ = Value_Number;
        return AddConstValue(v);
    }
    
    int Function::AddConstString(String* string) {
        Value v;
        v.str_ = string;
        v.str_->AddRef();
        v.type_ = Value_String;
        return AddConstValue(v);
    }
    
    int Function::AddConstBoolean(bool boolean) {
        Value v;
        v.boolean_ = boolean;
        v.type_ = Value_Boolean;
        return AddConstValue(v);
    }
    
    int Function::AddConstValue(const Value& v) {
        for(size_t i = 0; i < const_values_.size(); i++) {
            if(const_values_[i] == v) {
                return i;
            }
        }
        const_values_.push_back(v);
        return const_values_.size() - 1;
    }
}
