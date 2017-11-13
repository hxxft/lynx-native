
#ifndef LYNX_LEPUS_VISITOR_H_
#define LYNX_LEPUS_VISITOR_H_

#include "lepus/syntax_tree.h"

namespace lepus {
    class Visitor {
    public:
        virtual ~Visitor() {}
        virtual void Visit(ChunkAST* ast, void* data) = 0;
        virtual void Visit(BlockAST* ast, void* data) = 0;
        virtual void Visit(ReturnStatementAST* ast, void* data) = 0;
        virtual void Visit(LiteralAST* ast, void* data) = 0;
        virtual void Visit(NamesAST* ast, void* data) = 0;
        virtual void Visit(BinaryExprAST* ast, void* data) = 0;
        virtual void Visit(UnaryExpression* ast, void* data) = 0;
        virtual void Visit(ExpressionListAST* ast, void* data) = 0;
        virtual void Visit(VariableAST* ast, void* data) = 0;
        virtual void Visit(VariableListAST* ast, void* data) = 0;
        virtual void Visit(FunctionStatementAST* ast, void* data) = 0;
        virtual void Visit(ForStatementAST* ast, void* data) = 0;
        virtual void Visit(DoWhileStatementAST* ast, void* data) = 0;
        virtual void Visit(BreakStatementAST* ast, void* data) = 0;
        virtual void Visit(WhileStatementAST* ast, void* data) = 0;
        virtual void Visit(IfStatementAST* ast, void* data) = 0;
        virtual void Visit(ElseStatementAST* ast, void* data) = 0;
        virtual void Visit(SwitchStatementAST* ast, void* data) = 0;
        virtual void Visit(CaseStatementAST* ast, void* data) = 0;
        virtual void Visit(AssignStatement* ast, void* data) = 0;
        virtual void Visit(MemberAccessorAST* ast, void* data) = 0;
        virtual void Visit(FunctionCallAST* ast, void* data) = 0;
    };
}
#endif
