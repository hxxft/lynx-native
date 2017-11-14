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
        int op_index_;
        LoopInfo(LoopJmpType type, int index)
            :type_(type), op_index_(index){}
    };
    
    struct LoopGenerate {
        Function* function_;
        base::ScopedPtr<LoopGenerate> parent_;
        std::vector<LoopInfo> loop_infos_; //jmp head or jmp tail, loop controller op index in function
        int loop_start_index_;
        LoopGenerate():function_(nullptr),parent_(),loop_infos_(), loop_start_index_(0){}
    };
    
    struct BlockGenerate {
        Function* function_;
        std::unordered_map<String*, int> variables_map_;
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
        int register_id_;
        
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
        top_level_variables_(nullptr),
          register_id_(0),
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
    private:
        void EnterFunction();
        void LeaveFunction();
        void EnterBlock();
        void LeaveBlock();
        void EnterLoop();
        void LeaveLoop();
        
        void InsertVariable(String* name, int register_id);
        int SearchVariable(String* name);
        int SearchVariable(String* name, FunctionGenerate* current);
        int SearchGlobal(String* name);
        int ManageUpvalues(String* name);
        
        void WriteLocalValue(LexicalOp op, int dst, int src);
        void WriteUpValue(LexicalOp op, int dst, int src);
        
        void AutomaticLocalValue(AutomaticType type, int dst, int src);
        void AutomaticUpValue(AutomaticType type, int dst, int src);
        
        int GenerateRegisiterId() {
            int register_id = current_function_->register_id_++;
            return register_id;
        }
        
        int CurrentRegisiterId() {
            return current_function_->register_id_;
        }
        
        int ResetRegisiterId(int register_id) {
            current_function_->register_id_ = register_id;
        }
        
        VMContext* context_;
        void* top_level_variables_;
        int register_id_;
        String* current_function_name_;
        base::ScopedPtr<FunctionGenerate> current_function_;
    };
}

#endif
