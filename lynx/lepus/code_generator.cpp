
#include "lepus/code_generator.h"

namespace lepus {
    void CodeGenerator::Visit(ChunkAST* ast, void* data) {
        
    }
    
    void CodeGenerator::Visit(BlockAST* ast, void* data) {
        
    }
    
    void CodeGenerator::Visit(ReturnStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(LiteralAST* ast, void* data){
//        if(ast->token().token_ == Token_Number) {
//            data_segment_.push_back(new Value);
//            current_function_->code_.push_back(Instruction(TypeOp_LoadConst, register_id_++, data_segment_.size()-1));
//        }else if(ast->token().token_ == Token_Id) {
//            
//        }
    }
    
    void CodeGenerator::Visit(NamesAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(BinaryExprAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(UnaryExpression* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(ExpressionListAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(VariableAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(VariableListAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(FunctionStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(WhileStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(IfStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(ElseStatementAST* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(AssignStatement* ast, void* data){
        
    }
    
    void CodeGenerator::Visit(MemberAccessorAST* ast, void* data){
        
    }
}
