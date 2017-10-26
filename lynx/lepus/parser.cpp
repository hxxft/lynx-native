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
            throw ParseException("Expect <EOF>", current_token_);
        }
        return new ChunkAST(block);
    }
    
    ASTree* Parser::ParseBlock() {
        BlockAST* block = new BlockAST;
        while(LookAhead().token_ != Token_EOF &&
              LookAhead().token_ != Token_Else &&
              LookAhead().token_ != Token_Elseif &&
              LookAhead().token_ != '}') {
            if(LookAhead().token_ == Token_Return) {
                block->return_statement().Reset(ParseReturnStatement());
            }else {
                ASTree* statement = ParseStatement();
                if(statement)
                    block->statements().push_back(statement);
            }
        }
        return block;
    }
    
    ASTree* Parser::ParseBlockSingleLine() {
        BlockAST* block = new BlockAST;
        if(LookAhead().token_ == Token_Return) {
            block->return_statement().Reset(ParseReturnStatement());
        }else {
            ASTree* statement = ParseStatement();
            if(statement)
                block->statements().push_back(statement);
        }
        return block;
    }
    
    ASTree* Parser::ParseReturnStatement() {
        NextToken();
        ReturnStatementAST* return_statement = new ReturnStatementAST;
        if(LookAhead().token_ != Token_EOF ||
            LookAhead().token_ != Token_Else ||
            LookAhead().token_ != Token_Elseif ||
            LookAhead().token_ != Token_End) {
                return return_statement;
            }
        if(LookAhead().token_ == ';') {
            NextToken();
        }else{
            return_statement->expressions().Reset();
        }
            
        return return_statement;
    }
    
    ASTree* Parser::ParseBreakStatement() {
        return new BreakStatementAST(NextToken());
    }
    
    ASTree* Parser::ParseDoStatement() {
        //TODO
        return nullptr;
    }
    
    ASTree* Parser::ParseWhileStatement() {
        NextToken();
        
        ASTree* condition = ParseExpression();
        if(NextToken().token_ != '{') {
            // signal line block
        }
        
        ASTree* block = ParseBlock();
        
        if(NextToken().token_ != '}') {
            //TODO
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
                throw ParseException("expect '}'", current_token_);
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
                throw ParseException("expect '}'", current_token_);
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
                throw ParseException("expect '}'", current_token_);
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
            throw ParseException("expect function name", LookAhead());
        }
        
        NextToken();
        if(LookAhead().token_ != '(') {
            throw ParseException("expect '(' ", LookAhead());
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
            throw ParseException("expect '{' ", current_token_);
        }
        NextToken();
        function->body().Reset(ParseBlock());
        
        if(LookAhead().token_ != '}') {
            throw ParseException("expect '}' ", current_token_);
        }
        NextToken();
        return function;
    }
    
    ASTree* Parser::ParseForStatement() {
        return nullptr;
    }
    
    ASTree* Parser::ParseVarStatement() {
        NextToken();
        VariableListAST* var_list = new VariableListAST;
        do {
            if(LookAhead().token_ != Token_Id) {
                //TODO ERROR
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
            if(LookAhead().token_ == '=') {
                NextToken();
                ast = ParseExpression();
            }
            return new AssignStatement(expr, ast);
                
        }else if(type == ExprType_FunctionCall) {
            return expr;
        }else{
            throw ParseException("error statement", current_token_);
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
            case Token_Do:
                return ParseDoStatement();
            case Token_While:
                return ParseWhileStatement();
            case Token_If:
                return ParseIfStatement();
            case Token_Function:
                return ParseFunctionStatement();
            case Token_For:
                return ParseForStatement();
            case Token_Var:
                return ParseVarStatement();
            default:
                return ParseOtherStatement();
        }
        
        return nullptr;
    }
    
    ASTree* Parser::ParseExpressionList() {
        ExpressionListAST* ast = new ExpressionListAST;
        while (NextToken().token_ != ')') {
            ast->expressions().push_back(ParseExpression());
        }
        return ast;
    }
    
    ASTree* Parser::ParseExpression(ASTree* left, int left_priority, Token token) {
        ASTree* expression = nullptr;
        if(LookAhead().token_ == '-' || LookAhead().token_ == Token_Not) {
            NextToken();
            UnaryExpression* expression= new UnaryExpression;
            expression->op_token() = current_token_;
            expression->expression().Reset(ParseExpression(nullptr, 90, Token()));
        }else if(IsPrimaryExpr(LookAhead().token_)){
            expression = ParsePrimaryExpr();
        }else{
            //TODO ERROR
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
            case Token_False:
            case Token_True:
            case Token_Number:
            case Token_String:
                expr = new LiteralAST(NextToken());
                break;
            case Token_Function:
                break;
            case Token_Id:
            case '(':
                expr = ParsePrefixExpr();
                break;
            default:
                break;
        }
        return expr;
    }
    
    ASTree* Parser::ParsePrefixExpr(ExprType* type){
        ASTree* expr = nullptr;
        NextToken();
        if(current_token_.token_ == '(') {
            expr = ParseExpression();
            if(type) *type = ExprType_Normal;
            if(LookAhead().token_ != ')')
                throw ParseException("expect )", LookAhead());
            NextToken();
        }else{
            expr = new LiteralAST(current_token_);
            if(type) *type = ExprType_Var;
        }
        return ParsePrefixExprEnd(expr, type);
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
            return ParsePrefixExprEnd(expr, type);
        }else {
            return expr;
        }
    }
    
    ASTree* Parser::ParseVar(ASTree* table) {
        NextToken(); // skip '.'
        if(current_token_.token_ == '.'){
            if(NextToken().token_ != Token_Id)
                throw ParseException("expect <id>", LookAhead());
            return new MemberAccessorAST(table, current_token_);
        }
        return nullptr;
    }
    
    ASTree* Parser::ParseNames() {
        if(NextToken().token_ != Token_Id) {
            //TODO ERROR
        }
        NamesAST* names = new NamesAST;
        names->names().push_back(current_token_);
        while(LookAhead().token_ == ',') {
            NextToken();
            if(NextToken().token_ != Token_Id) {
                //TODO ERROR
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
