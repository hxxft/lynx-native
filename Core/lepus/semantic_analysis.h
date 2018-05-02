
#ifndef LYNX_LEPUS_SEMANTIC_ANALYSIS_H_
#define LYNX_LEPUS_SEMANTIC_ANALYSIS_H_

#include "lepus/syntax_tree.h"
#include "lepus/visitor.h"
#include "lepus/lepus_string.h"

#include <unordered_set>
#include <string>

namespace {
    struct LexicalBlock {
        base::ScopedPtr<LexicalBlock> parent_;
        std::unordered_set<lepus::String*> names_;
        
        LexicalBlock() : parent_(nullptr) {}
    };
    
    struct LexicalFunction {
        base::ScopedPtr<LexicalFunction> parent_;
        base::ScopedPtr<LexicalBlock> current_block_;
        
        LexicalFunction() : parent_(nullptr), current_block_(nullptr) {}
    };
}

namespace lepus {
    class SemanticAnalysis : public Visitor{
    public:
        SemanticAnalysis() {}
        virtual ~SemanticAnalysis(){}
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
        
        bool InsertName(const String* name);
        LexicalScoping SearchName(const String* name);
        
        base::ScopedPtr<LexicalFunction> current_function_;
    };
}


#endif
