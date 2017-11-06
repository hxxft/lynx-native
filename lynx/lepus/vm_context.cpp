
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
        (a->type_ == ValueT_Upvalue ? nullptr : a)
    
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
            root->Accept(&code_generator, nullptr);
        }catch(const lepus::Exception& exception) {
            std::cout<<exception.message()<<std::endl;
        }
        CallFunction(heap().top_ - 1, 0 , 0);
        Run();
    }
    
    Value VMContext::Call(const std::string& name, const std::vector<Value>& args) {
        
    }
    
    
    int VMContext::GetParamsSize() {
        return heap().top_ - frames_.back().register_;
    }
    
    Value* VMContext::GetParam(int index) {
        return frames_.back().register_ + index;
    }
    
    
    
    void VMContext::CallFunction(Value* function, int argc, int result_count) {
        heap_.top_ = function + 1 + argc;
        if(function->type_ == ValueT_Closure) {
            Frame frame;
            frame.function_ = function;
            frame.instuction_ = static_cast<Function*>(function->closure_)->GetOpCodes();
            frame.end_ = frame.instuction_ + static_cast<Function*>(function->closure_)->OpCodeSize();
            frame.register_ = heap_.top_;
            frames_.push_back(frame);
        }else if(function->type_ == ValueT_CFunction) {
            Frame frame;
            frame.function_ = function;
            frame.register_ = function + 1;
            frames_.push_back(frame);
            void* cfunction = function->native_function_;
            reinterpret_cast<CFunction>(cfunction)(this);
            frames_.pop_back();
        }
    }
    
    void VMContext::Run() {
        while(!frames_.empty()) {
            RunFrame();
        }
    }
    
    void VMContext::RunFrame() {
        Frame *frame = &frames_.back();
        Function *function = static_cast<Function*>(frame->function_->closure_);
        Value *a = nullptr;
        Value *b = nullptr;
        Value *c = nullptr;
        while(frame->instuction_ < frame->end_) {
            Instruction i = *frame->instuction_++;
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
                    break;
                case TypeOp_SetUpvalue:
                    break;
                case TypeOp_GetGlobal:
                    a = GET_REGISTER_A(i);
                    b = GET_Global_VALUE(i);
                    *GET_REAL_VALUE(a) = *b;
                    break;
                case TypeOp_SetGlobal:
                    break;
                case TypeOp_Closure:
                    
                    break;
                case TypeOp_Call:
                {
                    a = GET_REGISTER_A(i);
                    int argc = Instruction::GetParamB(i);
                    int resultc = Instruction::GetParamC(i);
                    CallFunction(a, argc, resultc);
                }
                    break;
                case TypeOp_VarArg:
                    break;
                case TypeOp_Ret:
                    break;
                case TypeOp_JmpFalse:
                    break;
                case TypeOp_JmpTrue:
                    break;
                case TypeOp_JmpNil:
                    break;
                case TypeOp_Jmp:
                    break;
                case TypeOp_Neg:
                    break;
                case TypeOp_Not:
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
                    break;
                case TypeOp_Greater:
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

}
