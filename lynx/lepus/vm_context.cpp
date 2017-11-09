
#include "lepus/vm_context.h"
#include "parser/input_stream.h"
#include "lepus/scanner.h"
#include "lepus/parser.h"
#include "lepus/exception.h"
#include "lepus/semantic_analysis.h"
#include "lepus/code_generator.h"
#include "lepus/value.h"
#include "lepus/builtin.h"

namespace lepus {

#define GET_CONST_VALUE(i) \
        (function->GetConstValue(Instruction::GetParamBx(i)))
#define GET_Global_VALUE(i) \
        (global().Get(Instruction::GetParamBx(i)))
#define GET_REGISTER_A(i)  \
        (frame->register_ + Instruction::GetParamA(i))
#define GET_REGISTER_B(i)  \
        (frame->register_ + Instruction::GetParamB(i))
#define GET_REGISTER_C(i)  \
        (frame->register_ + Instruction::GetParamC(i))
#define GET_REAL_VALUE(a) \
        (a)
#define GET_UPVALUE_B(i)  (closure->GetUpvalue(Instruction::GetParamB(i)))
    
#define GET_REGISTER_ABC(i)                                 \
    a = GET_REGISTER_A(i);                                  \
    b = GET_REGISTER_B(i);                                  \
    c = GET_REGISTER_C(i);
    
typedef void (*CFunction)(Context *);
    
    void VMContext::Initialize() {
        RegisterBulitin(this);
    }
    
    void VMContext::Execute(const std::string& source) {
        parser::InputStream input;
        input.Write(source);
        Scanner scanner(&input, &string_pool_);
        Parser parser(&scanner);
        SemanticAnalysis semantic_analysis;
        CodeGenerator code_generator(this);
        ASTree* root = nullptr;
        try {
            root = parser.Parse();
            root->Accept(&semantic_analysis, nullptr);
            root->Accept(&code_generator, &top_level_variables_);
        }catch(const lepus::Exception& exception) {
            std::cout<<exception.message()<<std::endl;
        }
        CallFunction(heap().top_ - 1, 0 , 0);
        Run();
    }
    
    Value VMContext::Call(const std::string& name, const std::vector<Value>& args) {
        auto reg_info = top_level_variables_.find(string_pool().NewString(name));
        if(reg_info == top_level_variables_.end())
            return Value();
        int reg = reg_info->second;
        Value* function = heap_.top_;
        *(heap_.top_++) = *(heap_.base() + reg + 1);
        for(int i = 0; i < args.size(); ++i) {
            *(heap_.top_++) = args[i];
        }
        CallFunction(function, args.size(), 0);
        Run();
    }
    
    
    int VMContext::GetParamsSize() {
        return heap().top_ - frames_.back().register_;
    }
    
    Value* VMContext::GetParam(int index) {
        return frames_.back().register_ + index;
    }
    
    
    
    bool VMContext::CallFunction(Value* function, int argc, int result_count) {
        if(function->type_ == ValueT_Closure) {
            heap_.top_ = function + 1;
            Frame frame;
            frame.function_ = function;
            frame.instruction_ = static_cast<Function*>(function->closure_->function())->GetOpCodes();
            frame.end_ = frame.instruction_ + static_cast<Function*>(function->closure_->function())->OpCodeSize();
            frame.register_ = heap_.top_;
            frames_.push_back(frame);
            return true;
        }else if(function->type_ == ValueT_CFunction) {
            heap_.top_ = function + argc + 1;
            Frame frame;
            frame.function_ = function;
            frame.register_ = function + 1;
            frames_.push_back(frame);
            void* cfunction = function->native_function_;
            reinterpret_cast<CFunction>(cfunction)(this);
            frames_.pop_back();
            return false;
        }
    }
    
    void VMContext::Run() {
        while(!frames_.empty()) {
            RunFrame();
        }
        heap_.top_ = heap_.base() + top_level_variables_.size() + 1;
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
                    break;
                case TypeOp_FillNil:
                    break;
                case TypeOp_LoadBool:
                    break;
                case TypeOp_LoadInt:
                    break;
                case TypeOp_LoadConst:
                    a = GET_REGISTER_A(i);
                    b = GET_CONST_VALUE(i);
                    *GET_REAL_VALUE(a) = *b;
                    break;
                case TypeOp_Move:
                    a = GET_REGISTER_A(i);
                    b = GET_REGISTER_B(i);
                    *GET_REAL_VALUE(a) = *GET_REAL_VALUE(b);
                    break;
                case TypeOp_GetUpvalue:
                    a = GET_REGISTER_A(i);
                    b = GET_UPVALUE_B(i);
                    *GET_REAL_VALUE(a) = *b;
                    break;
                case TypeOp_SetUpvalue:
                    a = GET_REGISTER_A(i);
                    b = GET_UPVALUE_B(i);
                    *b = *a;
                    break;
                case TypeOp_GetGlobal:
                    a = GET_REGISTER_A(i);
                    b = GET_Global_VALUE(i);
                    *GET_REAL_VALUE(a) = *b;
                    break;
                case TypeOp_SetGlobal:
                    break;
                case TypeOp_Closure:
                {
                    a = GET_REGISTER_A(i);
                    int index = Instruction::GetParamBx(i);
                    GenerateClosure(a, index);
                }
                    break;
                case TypeOp_Call:
                {
                    a = GET_REGISTER_A(i);
                    int argc = Instruction::GetParamB(i);
                    int resultc = Instruction::GetParamC(i);
                    heap_.top_ = a;
                    if(CallFunction(a, argc, resultc))
                        return;
                }
                    break;
                case TypeOp_VarArg:
                    break;
                case TypeOp_Ret:
                    break;
                case TypeOp_JmpFalse:
                    a = GET_REGISTER_A(i);
                    if (GET_REAL_VALUE(a)->IsFalse())
                        frame->instruction_ += -1 + Instruction::GetParamsBx(i);
                    break;
                case TypeOp_JmpTrue:
                    break;
                case TypeOp_JmpNil:
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
                    a->number_ = b->number_ + c->number_;
                    a->type_ = Value_Number;
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
                    a->number_ = int(b->number_) % int(c->number_);
                    a->type_ = Value_Number;
                    break;
                case TypeOp_And:
                    break;
                case TypeOp_Or:
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
                    break;
                case TypeOp_UnEqual:
                    break;
                case TypeOp_LessEqual:
                    break;
                case TypeOp_GreaterEqual:
                    break;
                case TypeOp_NewTable:
                    break;
                case TypeOp_SetTable:
                    break;
                case TypeOp_GetTable:
                    break;
                case TypeOp_ForInit:
                    break;
                case TypeOp_ForStep:
                    break;
                default:
                    break;
            }
        }
        frames_.pop_back();
    }
    
    void VMContext::GenerateClosure(Value* value, int index) {
        Frame* frame = &frames_.back();
        Closure* current_closure = frame->function_->closure_;
        Function *function = current_closure->function()->GetChildFunction(index);
        
        Closure* closure = new Closure(function);
        closure->AddRef();
        
        int upvalues_count = function->UpvaluesSize();
        for(int i = 0; i < upvalues_count; ++i) {
            UpvalueInfo* info = function->GetUpvalue(i);
            if(info->in_parent_vars_) {
                Value* v = frame->register_ + info->register_;
                closure->AddUpvalue(v);
            }else{
                closure->AddUpvalue(current_closure->GetUpvalue(info->register_));
            }
        }
        
        value->type_ = ValueT_Closure;
        value->closure_ = closure;
    }

}
