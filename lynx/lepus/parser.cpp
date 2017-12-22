//
//  parser.cpp
//  lynx
//
//  Created by dli on 2017/10/16.
//  Copyright © 2017年 lynx. All rights reserved.
//

#include "lepus/parser.h"

#include "lepus/syntax_tree.h"
#include "lepus/utils.h"
#include "lepus/exception.h"

namespace lepus {
    
    ASTree*Parser::Parse() {
        return ParseChunk();
    }
    
    ASTree* Parser::ParseChunk() {
        ASTree* block = ParseBlock();
        if(NextToken().token_ != Token_EOF) {
            throw CompileException("Expect <EOF>", current_token_);
        }
        return new ChunkAST(block);
    }
    
    ASTree* Parser::ParseBlock() {
        BlockAST* block = new BlockAST;
        while(LookAhead().token_ != Token_EOF &&
              LookAhead().token_ != Token_Else &&
              LookAhead().token_ != Token_Elseif &&
              LookAhead().token_ != Token_Case &&
              LookAhead().token_ != Token_Defalut &&
              LookAhead().token_ != '}') {
            ASTree* statement = ParseStatement();
            if(statement)
                block->statements().push_back(statement);
        }
        return block;
    }
    
    ASTree* Parser::ParseBlockSingleLine() {
        BlockAST* block = new BlockAST;
        ASTree* statement = ParseStatement();
        if(statement)
            block->statements().push_back(statement);
        return block;
    }
    
    ASTree* Parser::ParseReturnStatement() {
        NextToken();
        ReturnStatementAST* return_statement = new ReturnStatementAST;
        return_statement->expression().Reset(ParseExpression());
        return return_statement;
    }
    
    ASTree* Parser::ParseBreakStatement() {
        return new BreakStatementAST(NextToken());
    }
    
    ASTree* Parser::ParseForStatement() {
        NextToken(); //skip for
        if(LookAhead().token_ != '(') {
            throw CompileException("expect '('", current_token_);
        }
        ForStatementAST* ast = new ForStatementAST;
        NextToken();
        ast->statement1().Reset(ParseStatement());
        if(ast->statement1().Get() != nullptr) {
            if(NextToken().token_ != ';') {
                throw CompileException("expect ';'", current_token_);
            }
        }
        ast->statement2().Reset(ParseExpression());
        if(ast->statement1().Get() != nullptr) {
            if(NextToken().token_ != ';') {
                throw CompileException("expect ';'", current_token_);
            }
        }
        do {
            ast->statement3().push_back(ParseStatement());
        }while(NextToken().token_ == ',');
        if(current_token_.token_ != ')') {
            throw CompileException("expect ')'", current_token_);
        }
        if(NextToken().token_ != '{') {
            ast->block().Reset(ParseBlockSingleLine());
            return ast;
        }
        
        ast->block().Reset(ParseBlock());
        
        if(NextToken().token_ != '}') {
            throw CompileException("expect '}'", current_token_);
        }
        return ast;
    }
    
    ASTree* Parser::ParseDoWhileStatement() {
        NextToken();
        
        ASTree* block = nullptr;
        
        if(NextToken().token_ != '{') {
            block = ParseBlockSingleLine();
        }else{
            block = ParseBlock();
            
            if(NextToken().token_ != '}') {
                throw CompileException("expect '}'", current_token_);
            }
        }
        
        if(NextToken().token_ != Token_While) {
            throw CompileException("expect 'while'", current_token_);
        }
        
        ASTree* condition = ParseExpression();
        
        if(NextToken().token_ != ';') {
            throw CompileException("expect ';'", current_token_);
        }
        
        return new DoWhileStatementAST(condition, block);
    }
    
    ASTree* Parser::ParseWhileStatement() {
        NextToken();
        
        ASTree* condition = ParseExpression();
        if(NextToken().token_ != '{') {
            // signal line block
            return new WhileStatementAST(condition, ParseBlockSingleLine());
        }
        
        ASTree* block = ParseBlock();
        
        if(NextToken().token_ != '}') {
            throw CompileException("expect '}'", current_token_);
        }
            
        return new WhileStatementAST(condition, block);
    }
    
    ASTree* Parser::ParseIfStatement() {
        NextToken();  // skip 'if'
        
        ASTree* condition = ParseExpression();
        ASTree* true_branch = nullptr;
        if(LookAhead().token_ == '{') {
            NextToken(); // skip '{'
            true_branch = ParseBlock();
            
            if(NextToken().token_ != '}') { //skip '}'
                throw CompileException("expect '}'", current_token_);
            }
        }else {
            true_branch = ParseBlockSingleLine();
        }
        
        
         ASTree* false_branch = nullptr;
        if(LookAhead().token_ == Token_Else)
            false_branch = ParseElseStatement();
        else if(LookAhead().token_ == Token_Elseif) {
            false_branch = ParseElseIfStatement();
        }
            
        return new IfStatementAST(condition, true_branch, false_branch);
    }
    
    ASTree* Parser::ParseElseStatement() {
        NextToken();  // skip 'else'
        
        ASTree* block = nullptr;
        if(LookAhead().token_ == '{') {
            NextToken(); // skip '{'
            block = ParseBlock();
            
            if(NextToken().token_ != '}') { //skip '}'
                throw CompileException("expect '}'", current_token_);
            }
        }else {
            block = ParseBlockSingleLine();
        }
        return new ElseStatementAST(block);
    }
    
    ASTree* Parser::ParseElseIfStatement() {
        NextToken();  // skip 'elseif'
        
        ASTree* condition = ParseExpression();
        ASTree* true_branch = nullptr;
        if(LookAhead().token_ == '{') {
            NextToken(); // skip '{'
            true_branch = ParseBlock();
            
            if(NextToken().token_ != '}') { //skip '}'
                throw CompileException("expect '}'", current_token_);
            }
        }else {
            true_branch = ParseBlockSingleLine();
        }
        
        
        ASTree* false_branch = nullptr;
        if(LookAhead().token_ == Token_Else)
            false_branch = ParseElseStatement();
        else if(LookAhead().token_ == Token_Elseif) {
            false_branch = ParseElseIfStatement();
        }
        return new IfStatementAST(condition, true_branch, false_branch);
    }
    
    ASTree* Parser::ParseFunctionStatement() {
        NextToken();
        if(LookAhead().token_ != Token_Id) {
            throw CompileException("expect function name", LookAhead());
        }
        
        NextToken();
        if(LookAhead().token_ != '(') {
            throw CompileException("expect '(' ", LookAhead());
        }
        
        FunctionStatementAST* function = new FunctionStatementAST(current_token_);
        
        NamesAST* names = new NamesAST;
        while(NextToken().token_ != ')') {
            if(current_token_.token_ == Token_Id) {
                names->names().push_back(current_token_);
            }
        }
        function->params().Reset(names);
        
        
        if(LookAhead().token_  != '{') {
            throw CompileException("expect '{' ", current_token_);
        }
        NextToken();
        function->body().Reset(ParseBlock());
        
        if(LookAhead().token_ != '}') {
            throw CompileException("expect '}' ", current_token_);
        }
        NextToken();
        return function;
    }
    
    ASTree* Parser::ParseSwitchStatement() {
        NextToken();
        ASTree* expression = ParseExpression();
        SwitchStatementAST* switch_statement = new SwitchStatementAST(expression);
        if(LookAhead().token_  != '{') {
            throw CompileException("expect '{' ", current_token_);
        }
        NextToken();
        CaseStatementAST* case_statement = nullptr;
        do{
            case_statement = static_cast<CaseStatementAST*>(ParseCaseStatement());
            if(case_statement) {
                switch_statement->cases().push_back(case_statement);
            }
        }while(LookAhead().token_ == Token_Case || LookAhead().token_ == Token_Defalut);
        if(LookAhead().token_  != '}') {
            throw CompileException("expect '}' ", current_token_);
        }
        NextToken();
        return switch_statement;
    }
    
    ASTree* Parser::ParseCaseStatement() {
        NextToken();
        if(current_token_.token_ != Token_Case &&
           current_token_.token_ != Token_Defalut) {
            throw CompileException("expect case/default ", current_token_);
        }
        bool is_default = current_token_.token_ == Token_Defalut ? true : false;
        Token key = is_default ? current_token_ : NextToken();
        if(LookAhead().token_  != ':') {
            throw CompileException("expect ':' ", current_token_);
        }
        NextToken();
        
        return new CaseStatementAST(is_default, key, ParseBlock());
    }
    
    ASTree* Parser::ParseVarStatement() {
        VariableListAST* var_list = new VariableListAST;
        do {
            NextToken();
            if(LookAhead().token_ != Token_Id) {
                throw CompileException("invalid assign", LookAhead());
            }
            VariableAST* var = new VariableAST;
            NextToken();
            var->identifier() = current_token_;
            if(LookAhead().token_ == '=') {
                NextToken();
                var->expression().Reset(ParseExpression());
            }
            var_list->variable_list().push_back(var);
        }while(LookAhead().token_ == ',');
        return var_list;
    }
    
    ASTree* Parser::ParseOtherStatement() {
        ExprType type;
        ASTree* expr = ParsePrefixExpr(&type);
        
        if(type == ExprType_Var) {
            ASTree* ast = nullptr;
            if(LookAhead().token_ == '=' ||
               LookAhead().token_ == Token_ASSIGN_ADD ||
               LookAhead().token_ == Token_ASSIGN_SUB ||
               LookAhead().token_ == Token_ASSIGN_MUL ||
               LookAhead().token_ == Token_ASSIGN_DIV ||
               LookAhead().token_ == Token_ASSIGN_MOD ||
               LookAhead().token_ == Token_ASSIGN_BIT_OR ||
               LookAhead().token_ == Token_ASSIGN_BIT_AND ||
               LookAhead().token_ == Token_ASSIGN_BIT_XOR) {
                Token assignment = NextToken();
                ast = ParseExpression();
                return new AssignStatement(assignment, expr, ast);
            }else{
                return expr;
            }
                
        }else if(type == ExprType_FunctionCall) {
            return expr;
        }else{
            throw CompileException("error statement", current_token_);
        }
        return nullptr;
    }
    
    ASTree* Parser::ParseStatement() {
        switch (LookAhead().token_)
        {
            case ';':
                NextToken();
                break;
            case Token_Break:
                return ParseBreakStatement();
            case Token_For:
                return ParseForStatement();
            case Token_Do:
                return ParseDoWhileStatement();
            case Token_While:
                return ParseWhileStatement();
            case Token_If:
                return ParseIfStatement();
            case Token_Function:
                return ParseFunctionStatement();
            case Token_Var:
                return ParseVarStatement();
            case Token_Return:
                return ParseReturnStatement();
            case Token_Switch:
                return ParseSwitchStatement();
            default:
                return ParseOtherStatement();
        }
        
        return nullptr;
    }
    
    ASTree* Parser::ParseExpressionList() {
        ExpressionListAST* ast = new ExpressionListAST;
        do {
            NextToken();
            ast->expressions().push_back(ParseExpression());
        }while (LookAhead().token_ == ',');
        return ast;
    }
    
    ASTree* Parser::ParseExpression(ASTree* left, int left_priority, Token token) {
        ASTree* expression = nullptr;
        if(LookAhead().token_ == '-' || LookAhead().token_ == '!') {
            NextToken();
            expression = new UnaryExpression;
            static_cast<UnaryExpression*>(expression)->op_token() = current_token_;
            static_cast<UnaryExpression*>(expression)->expression().Reset(ParseExpression(new ASTree, 90, Token()));
        }else if(IsPrimaryExpr(LookAhead().token_)){
            expression = ParsePrimaryExpr();
        }else{
            throw CompileException("error expression", LookAhead());
            
        }
        
        while (true) {
            int right_priority = Priority(LookAhead().token_);
            if(left_priority < right_priority) {
                expression = ParseExpression(expression, right_priority, NextToken());
            }else if(left_priority == 0){
                return expression;
            }else if(left_priority == right_priority) {
                expression = new BinaryExprAST(left, expression, token);
                return ParseExpression(expression, right_priority, NextToken());
            }else {
                return new BinaryExprAST(left, expression, token);
            }
        }
        
        return expression;
    }
    
    ASTree* Parser::ParsePrimaryExpr() {
        ASTree* expr = nullptr;
        switch (LookAhead().token_) {
            case Token_Nil:
                expr = new LiteralAST(NextToken());
                break;
            case Token_Function:
                break;
            case Token_Id:
            case Token_False:
            case Token_True:
            case Token_Number:
            case Token_String:
            case '(':
            case Token_INC:
            case Token_DEC:
                expr = ParsePrefixExpr();
                break;
            default:
                break;
        }
        return expr;
    }
    
    ASTree* Parser::ParsePrefixExpr(ExprType* type){
        ASTree* expr = nullptr;
        AutomaticType auto_type = Automatic_None;
        if(LookAhead().token_ == '(') {
            NextToken();
            expr = ParseExpression();
            if(type) *type = ExprType_Normal;
            if(LookAhead().token_ != ')')
                throw CompileException("expect )", LookAhead());
            NextToken();
        }else if(LookAhead().token_ == Token_INC ||
                 LookAhead().token_ == Token_DEC ){
            auto_type = NextToken().token_ == Token_INC ?
                        Automatic_Inc_Before : Automatic_Dec_Before;
            expr = ParseExpression();
            if(type) *type = ExprType_Var;
        } else {
            expr = new LiteralAST(LookAhead());
            if(type) *type = ExprType_Var;
            NextToken();
        }
        expr = ParsePrefixExprEnd(expr, type);
        
        if(auto_type != Automatic_None) {
            if(expr->type() == ASTType_Literal) {
                static_cast<LiteralAST*>(expr)->auto_type() = auto_type;
            }else if(expr->type() == ASTType_MemberAccessor){
            }
        }
        
        return expr;
    }
    
    ASTree* Parser::ParsePrefixExprEnd(ASTree* expr, ExprType* type){
        if(LookAhead().token_ == '['
           || LookAhead().token_ == '.') {
            expr = ParseVar(expr);
            if(type) *type = ExprType_Var;
            return ParsePrefixExprEnd(expr, type);
        }else if(LookAhead().token_ == '(') {
            if(type) *type = ExprType_FunctionCall;
            expr = ParseFunctionCall(expr);
            NextToken();
            return ParsePrefixExprEnd(expr, type);
        }else if(LookAhead().token_ == Token_INC ||
                 LookAhead().token_ == Token_DEC ){
            if(expr->type() == ASTType_Literal) {
                static_cast<LiteralAST*>(expr)->auto_type() =
                    NextToken().token_ == Token_INC ? Automatic_Inc_After : Automatic_Dec_After;
            }else if(expr->type() == ASTType_MemberAccessor){
            }
            return expr;
        }else if(LookAhead().token_ == '?'){
            return ParseTernaryOperation(expr);
        }else{
            return expr;
        }
    }
    
    ASTree* Parser::ParseTernaryOperation(ASTree* condition){
        NextToken();
        ASTree* exprTrue = ParseExpression();
        if(LookAhead().token_ != ':'){
            throw CompileException("except : ", LookAhead());
        }
        NextToken();
        ASTree* exprFalse = ParseExpression();
        return new TernaryStatementAST(condition, exprTrue, exprFalse);
    }
    
    ASTree* Parser::ParseVar(ASTree* table) {
        NextToken(); // skip '.'
        if(current_token_.token_ == '.'){
            if(NextToken().token_ != Token_Id)
                throw CompileException("expect <id>", LookAhead());
            return new MemberAccessorAST(table, current_token_);
        }
        return nullptr;
    }
    
    ASTree* Parser::ParseNames() {
        if(NextToken().token_ != Token_Id) {
            throw CompileException("error name", current_token_);
        }
        NamesAST* names = new NamesAST;
        names->names().push_back(current_token_);
        while(LookAhead().token_ == ',') {
            NextToken();
            if(NextToken().token_ != Token_Id) {
                throw CompileException("error name", current_token_);
            }
            names->names().push_back(current_token_);
        }
        return names;
    }
    
    ASTree* Parser::ParseFunctionCall(ASTree* caller) {
        ASTree* args = ParseExpressionList();
        return new FunctionCallAST(caller, args);
    }
    
}
