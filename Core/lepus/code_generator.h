#ifndef LYNX_LEPUS_CODE_GENERATOR_H_
#define LYNX_LEPUS_CODE_GENERATOR_H_

#include "lepus/visitor.h"
#include "lepus/op_code.h"
#include "lepus/value.h"
#include "lepus/function.h"
#include "lepus/vm_context.h"

#include <vector>
#include <unordered_map>

namespace lepus {
    
    struct VariableNameInfo {
        int register_id_;
        int block_beigin_pc_;
        
        explicit VariableNameInfo(int register_id = 0, int begin_pc = 0)
        : register_id_(register_id),
        block_beigin_pc_(begin_pc){
        }
    };
    
    enum LoopJmpType {
        LoopJmp_Head,
        LoopJmp_Tail,
    };
    
    struct LoopInfo {
        LoopJmpType type_;
        long op_index_;
        LoopInfo(LoopJmpType type, long index)
            :type_(type), op_index_(index){}
    };
    
    struct LoopGenerate {
        Function* function_;
        base::ScopedPtr<LoopGenerate> parent_;
        std::vector<LoopInfo> loop_infos_; //jmp head or jmp tail, loop controller op index in function
        size_t loop_start_index_;
        LoopGenerate():function_(nullptr),parent_(),loop_infos_(), loop_start_index_(0){}
    };
    
    struct BlockGenerate {
        Function* function_;
        std::unordered_map<String*, long> variables_map_;
        base::ScopedPtr<BlockGenerate> parent_;
        int register_id_;
        BlockGenerate():function_(nullptr), variables_map_(),
            parent_(), register_id_(0){}
    };
    
    struct FunctionGenerate {
        base::ScopedPtr<FunctionGenerate> parent_;
        base::ScopedPtr<BlockGenerate> current_block_;
        base::ScopedPtr<LoopGenerate> current_loop_;
        Function* function_;
        long register_id_;
        
        FunctionGenerate():parent_(),
                    current_block_(),
                    current_loop_(),
                    function_(nullptr),
                    register_id_(0){}
    };
    

    
    class CodeGenerator : public Visitor{
    public:
        CodeGenerator(VMContext* context)
        :context_(context),
          register_id_(-1),
        current_function_name_(nullptr),
        current_function_(){
        }
        
        virtual ~CodeGenerator(){}
        virtual void Visit(ChunkAST* ast, void* data);
        virtual void Visit(BlockAST* ast, void* data);
        virtual void Visit(ReturnStatementAST* ast, void* data);
        virtual void Visit(LiteralAST* ast, void* data);
        virtual void Visit(NamesAST* ast, void* data);
        virtual void Visit(BinaryExprAST* ast, void* data);
        virtual void Visit(UnaryExpression* ast, void* data);
        virtual void Visit(ExpressionListAST* ast, void* data);
        virtual void Visit(VariableAST* ast, void* data);
        virtual void Visit(VariableListAST* ast, void* data);
        virtual void Visit(FunctionStatementAST* ast, void* data);
        virtual void Visit(ForStatementAST* ast, void* data);
        virtual void Visit(DoWhileStatementAST* ast, void* data);
        virtual void Visit(BreakStatementAST* ast, void* data);
        virtual void Visit(WhileStatementAST* ast, void* data);
        virtual void Visit(IfStatementAST* ast, void* data);
        virtual void Visit(ElseStatementAST* ast, void* data);
        virtual void Visit(SwitchStatementAST* ast, void* data);
        virtual void Visit(CaseStatementAST* ast, void* data);
        virtual void Visit(AssignStatement* ast, void* data);
        virtual void Visit(MemberAccessorAST* ast, void* data);
        virtual void Visit(FunctionCallAST* ast, void* data);
        virtual void Visit(TernaryStatementAST* ast, void* data);
    private:
        void EnterFunction();
        void LeaveFunction();
        void EnterBlock();
        void LeaveBlock();
        void EnterLoop();
        void LeaveLoop();
        void EnterRegister();
        void LeaveRegister();
        
        void InsertVariable(String* name, long register_id);
        long SearchVariable(String* name);
        long SearchVariable(String* name, FunctionGenerate* current);
        long SearchGlobal(String* name);
        long ManageUpvalues(String* name);
        
        void WriteLocalValue(LexicalOp op, long dst, long src);
        void WriteUpValue(LexicalOp op, long dst, long src);
        
        void AutomaticLocalValue(AutomaticType type, long dst, long src);
        void AutomaticUpValue(AutomaticType type, long dst, long src);
        
        long GenerateRegisiterId() {
            long register_id = current_function_->register_id_++;
            return register_id;
        }
        
        long CurrentRegisiterId() {
            return current_function_->register_id_;
        }
        
        void ResetRegisiterId(long register_id) {
            current_function_->register_id_ = register_id;
        }
        
        VMContext* context_;
        long register_id_;
        String* current_function_name_;
        base::ScopedPtr<FunctionGenerate> current_function_;
    };
}

#endif
