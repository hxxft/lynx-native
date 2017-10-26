#ifndef LYNX_LEPUS_CODE_GENERATOR_H_
#define LYNX_LEPUS_CODE_GENERATOR_H_

#include "lepus/visitor.h"
#include "lepus/op_code.h"
#include "lepus/value.h"
#include "lepus/function.h"

#include <vector>
#include <unordered_map>

namespace lepus {
    
    struct BlockGenerate {
        Function* function_;
        std::unordered_map<std::string, int> names_map_;
        BlockGenerate* parent_;
        int regiest_id_;
    };
    
    struct FunctionGenerate {
        base::ScopedPtr<FunctionGenerate> parent_;
        base::ScopedPtr<BlockGenerate> current_block_;
        Function* function_;
    };
    

    
    class CodeGenerator : public Visitor{
    public:
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
        virtual void Visit(BreakStatementAST* ast, void* data);
        virtual void Visit(WhileStatementAST* ast, void* data);
        virtual void Visit(IfStatementAST* ast, void* data);
        virtual void Visit(ElseStatementAST* ast, void* data);
        virtual void Visit(AssignStatement* ast, void* data);
        virtual void Visit(MemberAccessorAST* ast, void* data);
    private:
        int register_id_;
        Function* current_function_;
        std::vector<Value*> data_segment_;
        std::vector<Function*> code_segements_;
    };
}

#endif
