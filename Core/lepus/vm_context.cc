
#include "lepus/vm_context.h"
#include "parser/input_stream.h"
#include "lepus/scanner.h"
#include "lepus/parser.h"
#include "lepus/exception.h"
#include "lepus/semantic_analysis.h"
#include "lepus/code_generator.h"
#include "lepus/value.h"
#include "lepus/builtin.h"
#include "lepus/table.h"
#include "lepus/string_util.h"

namespace lepus {

#define GET_CONST_VALUE(i) \
        (function->GetConstValue(Instruction::GetParamBx(i)))
#define GET_Global_VALUE(i) \
        (global()->Get(Instruction::GetParamBx(i)))
#define GET_REGISTER_A(i)  \
        (frame->register_ + Instruction::GetParamA(i))
#define GET_REGISTER_B(i)  \
        (frame->register_ + Instruction::GetParamB(i))
#define GET_REGISTER_C(i)  \
        (frame->register_ + Instruction::GetParamC(i))

#define GET_UPVALUE_B(i)  (closure->GetUpvalue(Instruction::GetParamB(i)))
    
#define GET_REGISTER_ABC(i)                                 \
    a = GET_REGISTER_A(i);                                  \
    b = GET_REGISTER_B(i);                                  \
    c = GET_REGISTER_C(i);
    
    VMContext::~VMContext() {
        std::unordered_map<String*, long>::iterator iter;
        for(iter = top_level_variables_.begin(); iter != top_level_variables_.end(); ++iter) {
            iter->first->Release();
        }
    }
    
    void VMContext::Initialize() {
        RegisterBuiltin(this);
    }
    
    void VMContext::Execute(const std::string& source) {
        try {
            parser::InputStream input;
            input.Write(source);
            Scanner scanner(&input, &string_pool_);
            Parser parser(&scanner);
            SemanticAnalysis semantic_analysis;
            CodeGenerator code_generator(this);
            base::ScopedPtr<ASTree> root;
            root.Reset(parser.Parse());
            root->Accept(&semantic_analysis, nullptr);
            root->Accept(&code_generator, &top_level_variables_);
        }catch(const lepus::Exception& exception) {
            std::cout<<exception.message()<<std::endl;
        }
        CallFunction(heap().top_ - 1, 0, nullptr);
        Run();
    }
    
    Value VMContext::Call(const std::string& name, const std::vector<Value>& args) {
        Value ret;
        base::ScopedRefPtr<String> scoped_str(string_pool()->NewString(name));
        auto reg_info = top_level_variables_.find(scoped_str.Get());
        if(reg_info == top_level_variables_.end())
            return Value();
        long reg = reg_info->second;
        Value* function = heap_.top_;
        *(heap_.top_++) = *(heap_.base() + reg + 1);
        for(std::size_t i = 0; i < args.size(); ++i) {
            *(heap_.top_++) = args[i];
        }
        CallFunction(function, args.size(), &ret);
        Run();
        return ret;
    }
    
    
    long VMContext::GetParamsSize() {
        return heap().top_ - frames_.back().register_;
    }
    
    Value* VMContext::GetParam(long index) {
        return frames_.back().register_ + index;
    }

    bool VMContext::UpdateTopLevelVariable(const std::string &name, Value value) {
        auto reg_info = top_level_variables_.find(string_pool()->NewString(name));
        if(reg_info == top_level_variables_.end())
            return false;
        long reg = reg_info->second;
        *(heap_.base() + reg + 1) = value;
        return true;
    }
    
    bool VMContext::CallFunction(Value* function, size_t argc, Value* ret) {
        if(function->type_ == Value_Closure) {
            heap_.top_ = function + 1;
            Frame frame;
            frame.return_ = ret;
            frame.function_ = function;
            frame.instruction_ = static_cast<Function*>(function->closure_->function())->GetOpCodes();
            frame.end_ = frame.instruction_ + static_cast<Function*>(function->closure_->function())->OpCodeSize();
            frame.register_ = heap_.top_;
            frames_.push_back(frame);
            return true;
        }else if(function->type_ == Value_CFunction) {
            heap_.top_ = function + argc + 1;
            Frame frame;
            frame.return_ = ret;
            frame.function_ = function;
            frame.register_ = function + 1;
            frames_.push_back(frame);
            void* cfunction = function->native_function_;
            Value v = reinterpret_cast<CFunction>(cfunction)(this);
            if(ret){
                *ret = v;
            }
            frames_.pop_back();
            return false;
        }
        return false;
    }
    
    void VMContext::Run() {
        try {
            while(!frames_.empty()) {
                RunFrame();
            }
            heap_.top_ = heap_.base() + top_level_variables_.size() + 1;
        } catch (const lepus::Exception& exception) {
            std::cout<<exception.message()<<std::endl;
        }
    }
    
    void VMContext::RunFrame() {
        Frame *frame = &frames_.back();
        Closure* closure = frame->function_->closure_;
        Function *function = closure->function();
        Value *a = nullptr;
        Value *b = nullptr;
        Value *c = nullptr;
        while(frame->instruction_ < frame->end_) {
            Instruction i = *frame->instruction_++;
            switch (Instruction::GetOpCode(i)) {
                case TypeOp_LoadNil:
                    a = GET_REGISTER_A(i);
                    a->SetNil();
                    break;
                case TypeOp_LoadConst:
                    a = GET_REGISTER_A(i);
                    b = GET_CONST_VALUE(i);
                    *a = *b;
                    break;
                case TypeOp_Move:
                    a = GET_REGISTER_A(i);
                    b = GET_REGISTER_B(i);
                    *a = *b;
                    break;
                case TypeOp_GetUpvalue:
                    a = GET_REGISTER_A(i);
                    b = GET_UPVALUE_B(i);
                    *a = *b;
                    break;
                case TypeOp_SetUpvalue:
                    a = GET_REGISTER_A(i);
                    b = GET_UPVALUE_B(i);
                    *b = *a;
                    break;
                case TypeOp_GetGlobal:
                    a = GET_REGISTER_A(i);
                    b = GET_Global_VALUE(i);
                    *a = *b;
                    break;
                case TypeOp_SetGlobal:
                    break;
                case TypeOp_Closure:
                {
                    a = GET_REGISTER_A(i);
                    long index = Instruction::GetParamBx(i);
                    GenerateClosure(a, index);
                }
                    break;
                case TypeOp_Call:
                {
                    a = GET_REGISTER_A(i);
                    long argc = Instruction::GetParamB(i);
                    c = GET_REGISTER_C(i);
                    if(CallFunction(a, argc, c))
                        return;
                }
                    break;
                case TypeOp_Ret:
                    a = GET_REGISTER_A(i);
                    if(frame->return_ != nullptr) {
                        *frame->return_ = *a;
                    }
                    frames_.pop_back();
                    return;
                case TypeOp_JmpFalse:
                    a = GET_REGISTER_A(i);
                    if (a->IsFalse())
                        frame->instruction_ += -1 + Instruction::GetParamsBx(i);
                    break;
                case TypeOp_Jmp:
                    frame->instruction_ += -1 + Instruction::GetParamsBx(i);
                    break;
                case TypeOp_Neg:
                    a = GET_REGISTER_A(i);
                    a->number_ = -a->number_;
                    break;
                case TypeOp_Not:
                    a = GET_REGISTER_A(i);
                    a->boolean_ = !a->boolean_;
                    break;
                case TypeOp_Len:
                    break;
                case TypeOp_Add:
                    GET_REGISTER_ABC(i);
                    // 判断是不是数字相加
                    if( b->type_ == Value_String || c->type_ == Value_String){
                        std::string b_string;
                        std::string c_string;
                        if(b->type_ == Value_Number){
                            b_string = to_string(b->number_);
                            DeleteZero(b_string);
                        }else{
                            b_string = b->str_->c_str();
                        }
                        if(c->type_ == Value_Number){
                            c_string = to_string(c->number_);
                            DeleteZero(c_string);
                        }else{
                            c_string = c->str_->c_str();
                        }
                        a->str_ = string_pool()->NewString(b_string + c_string);
                        a->str_->AddRef();
                        a->type_ = Value_String;
                    }else{
                        a->number_ = b->number_ + c->number_;
                        a->type_ = Value_Number;
                    }
                    break;
                case TypeOp_Sub:
                    GET_REGISTER_ABC(i);
                    a->number_ = b->number_ - c->number_;
                    a->type_ = Value_Number;
                    break;
                case TypeOp_Mul:
                    GET_REGISTER_ABC(i);
                    a->number_ = b->number_ * c->number_;
                    a->type_ = Value_Number;
                    break;
                case TypeOp_Div:
                    GET_REGISTER_ABC(i);
                    a->number_ = b->number_ / c->number_;
                    a->type_ = Value_Number;
                    break;
                case TypeOp_Pow:
                    break;
                case TypeOp_Mod:
                    GET_REGISTER_ABC(i);
                    a->number_ = int(b->number_/c->number_);
                    a->number_ = b->number_ - a->number_ * c->number_;
                    a->type_ = Value_Number;
                    break;
                case TypeOp_And:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Boolean)
                        a->boolean_ = (b->boolean_ && c->boolean_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_Or:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Boolean)
                        a->boolean_ = (b->boolean_ || c->boolean_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_Less:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Number)
                        a->boolean_ = (b->number_ < c->number_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_Greater:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Number)
                        a->boolean_ = (b->number_ > c->number_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_Equal:
                    GET_REGISTER_ABC(i);
                    a->boolean_ = (*b == *c);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_UnEqual:
                    GET_REGISTER_ABC(i);
                    a->boolean_ = (*b != *c);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_LessEqual:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Number)
                        a->boolean_ = (b->number_ <= c->number_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_GreaterEqual:
                    GET_REGISTER_ABC(i);
                    if (b->type_ == Value_Number)
                        a->boolean_ = (b->number_ >= c->number_);
                    a->type_ = Value_Boolean;
                    break;
                case TypeOp_NewTable:
                    break;
                case TypeOp_SetTable:
                    break;
                case TypeOp_GetTable:
                    GET_REGISTER_ABC(i);
                    if(b->type_ == Value_Table && c->type_ == Value_String) {
                        *a = static_cast<Dictonary*>(b->table_)->GetValue(c->str_);
                    }else if(b->type_ == Value_String && c->type_ == Value_String){
                        Value* v = global()->Find(string_pool()->NewString("String"));
                        *a = static_cast<Dictonary*>(v->table_)->GetValue(c->str_);
                    }
                    
                    break;
                case TypeOp_Switch:
                {
                    a = GET_REGISTER_A(i);
                    long index = Instruction::GetParamBx(i);
                    long jmp = function->GetSwitch(index)->Switch(a);
                    frame->instruction_ += -1 + jmp;
                }
                    break;
                case TypeOp_Inc:
                    a = GET_REGISTER_A(i);
                    if (a->type_ == Value_Number)
                        a->number_ += 1;
                    break;
                case TypeOp_Dec:
                    a = GET_REGISTER_A(i);
                    if (a->type_ == Value_Number)
                        a->number_ -= 1;
                    break;
                case TypeOp_Noop:
                    break;
                default:
                    break;
            }
        }
        if(frame->return_ != nullptr) {
            frame->return_->SetNil();
        }
        frames_.pop_back();
    }
    
    void VMContext::GenerateClosure(Value* value, long index) {
        Frame* frame = &frames_.back();
        Closure* current_closure = frame->function_->closure_;
        Function *function = current_closure->function()->GetChildFunction(index);
        
        Closure* closure = lynx_new Closure(function);
        
        std::size_t upvalues_count = function->UpvaluesSize();
        for(int i = 0; i < upvalues_count; ++i) {
            UpvalueInfo* info = function->GetUpvalue(i);
            if(info->in_parent_vars_) {
                Value* v = frame->register_ + info->register_;
                closure->AddUpvalue(v);
            }else{
                closure->AddUpvalue(current_closure->GetUpvalue(info->register_));
            }
        }
        
        value->type_ = Value_Closure;
        value->closure_ = closure;
    }

}
