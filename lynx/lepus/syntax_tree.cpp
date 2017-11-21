
#include "lepus/syntax_tree.h"
#include "lepus/visitor.h"

namespace lepus {
    
    void ChunkAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void BlockAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void ReturnStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void LiteralAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void NamesAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void BinaryExprAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void UnaryExpression::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void ExpressionListAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void VariableAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void VariableListAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void FunctionStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void DoWhileStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void ForStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void BreakStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void WhileStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void IfStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void ElseStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void SwitchStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void CaseStatementAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void AssignStatement::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void MemberAccessorAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
    
    void FunctionCallAST::Accept(Visitor *visitor, void* data) {
        visitor->Visit(this, data);
    }
}
