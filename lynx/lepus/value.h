#ifndef LYNX_LEPUS_VALUE_H_
#define LYNX_LEPUS_VALUE_H_

#include "lepus/string.h"
#include "lepus/function.h"

namespace lepus {
    enum ValueType {
        Value_Nil,
        Value_Number,
        Value_Boolean,
        Value_String,
        Value_Table,
        ValueT_Closure,
        ValueT_CFunction,
    };
    
    class Value {
    public:
        union {
            double number_;
            bool boolean_;
            String* str_;
            Closure* closure_;
            void* native_function_;
            void* table_;
        };
        ValueType type_;
        
        Value():number_(0), type_(Value_Nil){}
        Value(double number):number_(number), type_(Value_Number){}
        Value(const Value& value) {
            Copy(value);
        }
        
        ~Value(){
            SetNil();
        }
        
        bool IsFalse() const
        { return type_ == Value_Nil || (type_ == Value_Boolean && !boolean_); }

        void SetNil() {
            if(this->type_ == Value_String ) {
                str_->Release();
            }else if(this->type_ == ValueT_Closure) {
                closure_->Release();
            }
            number_ = 0;
            type_ = Value_Nil;
        }
        
        Value& operator= (const Value& value) {
            if(*this == value) {
                return *this;
            }
            if(this->type_ == Value_String) {
                str_->Release();
            }else if(this->type_ == ValueT_Closure){
                closure_->Release();
            }
            
            Copy(value);
            
            return *this;
        }
    
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
                    return left.str_ == right.str_;
                case ValueT_Closure:
                    return left.closure_ == right.closure_;
                case ValueT_CFunction:
                    return left.native_function_ == right.native_function_;
                default:
                    break;
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
        
    private:
        inline void Copy(const Value& value) {
            switch (value.type_) {
                case Value_Nil:
                    this->str_ = nullptr;
                    this->type_ = Value_Nil;
                    break;
                case Value_Number:
                    this->number_ = value.number_;
                    this->type_ = Value_Number;
                    break;
                case Value_Boolean:
                    this->boolean_ = value.boolean_;
                    this->type_ = Value_Boolean;
                    break;
                case Value_String:
                    this->str_ = value.str_;
                    this->str_->AddRef();
                    this->type_ = Value_String;
                    break;
                case ValueT_Closure:
                    this->closure_ = value.closure_;
                    this->closure_->AddRef();
                    this->type_ = ValueT_Closure;
                    break;
                case ValueT_CFunction:
                    this->native_function_ = value.native_function_;
                    this->type_ = ValueT_CFunction;
                    break;
                default:
                    this->str_ = nullptr;
                    this->type_ = Value_Nil;
                    break;
            }
        }
    };
}

#endif // LYNX_LEPUS_VALUE_H_
