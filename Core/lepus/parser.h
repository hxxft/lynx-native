//
//  parser.hpp
//  lynx
//
//  Created by dli on 2017/10/16.
//  Copyright © 2017年 lynx. All rights reserved.
//

#ifndef LYNX_LEPUS_PARSER_H_
#define LYNX_LEPUS_PARSER_H_

#include "lepus/scanner.h"
namespace {
    enum ExprType {
        ExprType_Normal,
        ExprType_Var,
        ExprType_FunctionCall,
    };
}
namespace lepus {
    class ASTree;
    class Parser {
    public:
        Parser(Scanner* scanner) : scanner_(scanner) {}
        ASTree* Parse();
        ASTree* ParseChunk();
        ASTree* ParseBlock();
        ASTree* ParseBlockSingleLine();
        ASTree* ParseReturnStatement();
        
        ASTree* ParseStatement();
        ASTree* ParseBreakStatement();
        ASTree* ParseForStatement();
        ASTree* ParseDoWhileStatement();
        ASTree* ParseWhileStatement();
        ASTree* ParseIfStatement();
        ASTree* ParseElseStatement();
        ASTree* ParseElseIfStatement();
        ASTree* ParseSwitchStatement();
        ASTree* ParseCaseStatement();
        ASTree* ParseOtherStatement();
        
        ASTree* ParseFunctionStatement();
        ASTree* ParseVarStatement();
        ASTree* ParseExpressionList();
        ASTree* ParseExpression(ASTree* left = nullptr, int left_priority = 0, Token token = Token());
        ASTree* ParsePrimaryExpr();
        ASTree* ParsePrefixExpr(ExprType* type = nullptr);
        ASTree* ParsePrefixExprEnd(ASTree* expr, ExprType* type);
        ASTree* ParseVar(ASTree* table);
        ASTree* ParseNames();
        ASTree* ParseFunctionCall(ASTree* caller);
        ASTree* ParseTernaryOperation(ASTree* condition);
        
    private:
        Token& NextToken() {
            if(next_token_.token_ != Token_EOF) {
                current_token_ = next_token_;
                next_token_ = next_next_token_;
                if(next_next_token_.token_ != Token_EOF) {
                    next_next_token_.token_ = Token_EOF;
                }
            }else {
                scanner_->NextToken(current_token_);
            }
            return current_token_;
        }
        
        Token& LookAhead() {
            if(next_token_.token_ == Token_EOF) {
                scanner_->NextToken(next_token_);
            }
            return next_token_;
        }
        
        Token& LookAhead2() {
            LookAhead();
            if(next_next_token_.token_ == Token_EOF) {
                scanner_->NextToken(next_next_token_);
            }
            return next_next_token_;
        }
        
        int Priority(int token) {
            switch (token)
            {
                case '^':
                    return 100;
                case '*':
                case '/':
                case '%':
                    return 80;
                case '+':
                case '-':
                    return 70;
                case '>':
                case '<':
                case Token_GreaterEqual:
                case Token_LessEqual:
                case Token_NotEqual:
                case Token_Equal:
                    return 50;
                case Token_And:
                    return 40;
                case Token_Or:
                    return 30;
                default:
                    return 0;
            }
        }
        
        Token current_token_;
        Token next_token_;
        Token next_next_token_;
        Scanner* scanner_;
    };
}

#endif  // LYNX_LEPUS_PARSER_H_
