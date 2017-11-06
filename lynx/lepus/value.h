#ifndef LYNX_LEPUS_VALUE_H_
#define LYNX_LEPUS_VALUE_H_

#include <string>

namespace lepus {
    enum ValueType {
        Value_Nil,
        Value_Number,
        Value_Boolean,
        Value_String,
        Value_Table,
        ValueT_Upvalue,
        ValueT_Closure,
        ValueT_CFunction,
    };
    
    class Closure;
    
    class Value {
    public:
        union {
            double number_;
            bool boolean_;
            char* str_;
            void* closure_;
            void* native_function_;
            void* table_;
        };
        ValueType type_;
        
        Value():number_(0), type_(Value_Nil){}
        Value(double number):number_(number), type_(Value_Number){}
    
        friend bool operator == (const Value& left, const Value& right) {
            if(left.type_ != right.type_) return false;
            switch (left.type_) {
                case Value_Nil:
                    return true;
                case Value_Number:
                    return left.number_ == right.number_;
                case Value_Boolean:
                    return left.boolean_ == right.boolean_;
                case Value_String:
                    return strcmp(left.str_, right.str_);
                case ValueT_Closure:
                    return left.closure_ == right.closure_;
                case ValueT_CFunction:
                    return left.native_function_ == right.native_function_;
            }
            return false;
        }
        
        friend bool operator != (const Value& left, const Value& right) {
            return !(left == right);
        }
        
        friend Value operator + (const Value& left, const Value& right) {
            Value value;
            if(left.type_ == Value_Number && right.type_ == Value_Number) {
                value.type_ = Value_Number;
                value.number_ = left.number_ + right.number_;
            }
            return value;
        }
        
        friend Value operator - (const Value& left, const Value& right) {
            Value value;
            if(left.type_ == Value_Number && right.type_ == Value_Number) {
                value.type_ = Value_Number;
                value.number_ = left.number_ - right.number_;
            }
            return value;
        }
        
        friend Value operator * (const Value& left, const Value& right) {
            Value value;
            if(left.type_ == Value_Number && right.type_ == Value_Number) {
                value.type_ = Value_Number;
                value.number_ = left.number_ * right.number_;
            }
            return value;
        }
        
        friend Value operator / (const Value& left, const Value& right) {
            Value value;
            if(left.type_ == Value_Number && right.type_ == Value_Number) {
                value.type_ = Value_Number;
                value.number_ = left.number_ / right.number_;
            }
            return value;
        }
        
        friend Value operator %(const Value& left, const Value& right) {
            Value value;
            if(left.type_ == Value_Number && right.type_ == Value_Number) {
                value.type_ = Value_Number;
                value.number_ = ((int)left.number_) % ((int)right.number_);
            }
            return value;
        }
        
        Value& operator += (const Value& value) {
            if(type_ == Value_Number && value.type_ == Value_Number) {
                number_ += value.number_;
            }
            return *this;
        }
        
        Value& operator -= (const Value& value) {
            if(type_ == Value_Number && value.type_ == Value_Number) {
                number_ -= value.number_;
            }
            return *this;
        }
        
        Value& operator *= (const Value& value) {
            if(type_ == Value_Number && value.type_ == Value_Number) {
                number_ *= value.number_;
            }
            return *this;
        }
        
        Value& operator /= (const Value& value) {
            if(type_ == Value_Number && value.type_ == Value_Number) {
                number_ /= value.number_;
            }
            return *this;
        }
        
        Value& operator %= (const Value& value) {
            if(type_ == Value_Number && value.type_ == Value_Number) {
                number_ = ((int)number_) % ((int)value.number_);
            }
            return *this;
        }
    };
}

#endif // LYNX_LEPUS_VALUE_H_
