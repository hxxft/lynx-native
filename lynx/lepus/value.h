#ifndef LYNX_LEPUS_VALUE_H_
#define LYNX_LEPUS_VALUE_H_

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
    
    struct Value {
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
//        
//        ~Value(){
//            switch (type_) {
//                case ValueT_Closure:
//                    delete static_cast<Closure*>(closure_);
//                    break;
//                case Value_String:
//                    delete[] str_;
//                    break;
//                default:
//                    break;
//            }
//        }
    };
    
    inline bool operator == (const Value& left, const Value& right) {
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
    
    inline bool operator != (const Value& left, const Value& right) {
        return !(left == right);
    }
}

#endif // LYNX_LEPUS_VALUE_H_
