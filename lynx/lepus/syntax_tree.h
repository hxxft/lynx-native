//
//  syntax_tree.h
//  lynx
//
//  Created by dli on 2017/10/16.
//  Copyright © 2017年 lynx. All rights reserved.
//

#ifndef LEPUS_SYNTAX_TREE_H_
#define LEPUS_SYNTAX_TREE_H_

#include "base/scoped_vector.h"
#include "base/scoped_ptr.h"
#include "lepus/token.h"

namespace lepus {
    
    class Visitor;
    
    enum LexicalScoping {
        LexicalScoping_Unknow,
        LexicalScoping_Global,
        LexicalScoping_Upvalue,
        LexicalScoping_Local,
    };
    
#define AST_ACCEPT_VISITOR \
virtual void Accept(Visitor* visitor, void* data);
    
    class ASTree {
    public:
        virtual ~ASTree() {}
        virtual void Accept(Visitor* visitor, void* data) = 0;
    };
    
    class ChunkAST : public ASTree {
    public:
        ChunkAST(ASTree* block) : block_(block) {}
        base::ScopedPtr<ASTree>& block(){
            return block_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> block_;;
    };
    
    class BlockAST : public ASTree {
    public:
        
        base::ScopedVector<ASTree>& statements() {
            return statements_;
        }
        base::ScopedPtr<ASTree>& return_statement() {
            return return_statement_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedVector<ASTree> statements_;
        base::ScopedPtr<ASTree> return_statement_;
    };
    
    class ReturnStatementAST : public ASTree {
    public:
        base::ScopedPtr<ASTree>& expressions() {
            return expressions_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> expressions_;
    };
    
    class LiteralAST : public ASTree {
    public:
        LiteralAST(const Token& token)
        :token_(token)
        ,scope_(LexicalScoping_Unknow){
            
        }
        
        Token& token() {
            return token_;
        }
        
        LexicalScoping& scope() {
            return scope_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        Token token_;
        LexicalScoping scope_;
    };
    
    class NamesAST : public ASTree {
    public:
        std::vector<Token>& names() {
            return names_;
        }
        AST_ACCEPT_VISITOR
    private:
        std::vector<Token> names_;
    };
    
    class BinaryExprAST : public ASTree {
    public:
        BinaryExprAST(){}
        BinaryExprAST(ASTree* left, ASTree* right, const Token& op)
        : left_(left), right_(right), op_token_(op){}
        
        base::ScopedPtr<ASTree>& left(){
            return left_;
        }
        
        base::ScopedPtr<ASTree>& right(){
            return right_;
        }
        
        Token& op_token() {
            return op_token_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> left_;
        base::ScopedPtr<ASTree> right_;
        Token op_token_;
    };
    
    class UnaryExpression : public ASTree {
    public:
        UnaryExpression(){}
        UnaryExpression(ASTree* expression, const Token& op)
        : expression_(expression), op_token_(op){
        }
        
        base::ScopedPtr<ASTree>& expression(){
            return expression_;
        }
        
        Token& op_token() {
            return op_token_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> expression_;
        Token op_token_;
    };
    
    class ExpressionListAST : public ASTree {
    public:
        base::ScopedVector<ASTree>& expressions() {
            return expressions_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedVector<ASTree> expressions_;
    };
    
    class VariableAST : public ASTree {
    public:
        VariableAST(){}
        VariableAST(const Token& identifier, ASTree* expression)
        :identifier_(identifier), expression_(expression){}
        
        Token& identifier() {
            return identifier_;
        }
        base::ScopedPtr<ASTree>& expression() {
            return expression_;
        }
        AST_ACCEPT_VISITOR
    private:
        Token identifier_;
        base::ScopedPtr<ASTree> expression_;
    };
    
    class VariableListAST : public ASTree {
    public:
        VariableListAST(){}
        base::ScopedVector<VariableAST>& variable_list() {
            return variable_list_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedVector<VariableAST> variable_list_;
    };
    
    class FunctionStatementAST : public ASTree {
    public:
        FunctionStatementAST() {}
        FunctionStatementAST(const Token& name)
        :function_name_(name){}
        
        base::ScopedPtr<ASTree>& params() {
            return params_;
        }
        
        base::ScopedPtr<ASTree>& body() {
            return body_;
        }
        
        Token& function_name() {
            return function_name_;
        }
         AST_ACCEPT_VISITOR
    private:
        Token function_name_;
        base::ScopedPtr<ASTree> params_;
        base::ScopedPtr<ASTree> body_;
    };
    
    class BreakStatementAST : public ASTree {
    public:
        explicit BreakStatementAST(const Token& token) : break_(token) {
            
        }
        AST_ACCEPT_VISITOR
    private:
        Token break_;
    };
    
    class WhileStatementAST : public ASTree {
    public:
        explicit WhileStatementAST(ASTree* condition, ASTree* block)
        : condition_(condition),
            block_(block) {
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> condition_;
        base::ScopedPtr<ASTree> block_;
    };
    
    class IfStatementAST : public ASTree {
    public:
        explicit IfStatementAST(ASTree* condition, ASTree* true_branch, ASTree* false_branch)
        : condition_(condition),
            true_branch_(true_branch),
            false_branch_(false_branch){
        }
        
        const base::ScopedPtr<ASTree>& condition() {
            return condition_;
        }
        
        const base::ScopedPtr<ASTree>& true_branch() {
            return true_branch_;
        }
        
        const base::ScopedPtr<ASTree>& false_branch() {
            return false_branch_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> condition_;
        base::ScopedPtr<ASTree> true_branch_;
        base::ScopedPtr<ASTree> false_branch_;
    };
    
    class ElseStatementAST : public ASTree {
    public:
        explicit ElseStatementAST(ASTree* block) : block_(block) {
            
        }
        
        const base::ScopedPtr<ASTree> block() {
            return block_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> block_;
    };
    
    class AssignStatement : public ASTree {
    public:
        AssignStatement(ASTree* variable, ASTree* expression)
        :variable_(variable), expression_(expression){
        }
        
        base::ScopedPtr<ASTree>& variable() {
            return variable_;
        }
        base::ScopedPtr<ASTree>& expression() {
            return expression_;
        }
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> variable_;
        base::ScopedPtr<ASTree> expression_;
    };
    
    class MemberAccessorAST : public ASTree {
    public:
        MemberAccessorAST(ASTree* table, const Token& member)
        : table_(table), member_(member){
            
        }
        
        base::ScopedPtr<ASTree>& table() {
            return table_;
        }
        
        Token& member() {
            return member_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> table_;
        Token member_;
    };
    
    class FunctionCallAST : public ASTree {
    public:
        FunctionCallAST(ASTree* caller, ASTree* args)
        : caller_(caller), args_(args){
            
        }
        
        base::ScopedPtr<ASTree>& caller(){
            return caller_;
        }
        
        base::ScopedPtr<ASTree>& args() {
            return args_;
        }
        
        AST_ACCEPT_VISITOR
    private:
        base::ScopedPtr<ASTree> caller_;
        base::ScopedPtr<ASTree> args_;
    };
}

#endif  // LEPUS_SYNTAX_TREE_H_
