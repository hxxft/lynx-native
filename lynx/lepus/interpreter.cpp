
#include "lepus/interpreter.h"
#include "lepus/table.h"
namespace lepus {
    
    class Closure {
    public:
        void SetFunction(FunctionStatementAST* function) {
            function_ = function;
        }
        
        void Run(Interpreter* parent, const std::vector<Value>& args) {
            base::ScopedPtr<Interpreter> interpreter(new Interpreter);
            interpreter->set_parent(parent);
            function_->params()->Accept(interpreter.Get(), static_cast<void*>(const_cast<std::vector<Value>*>(&args)));
            function_->body()->Accept(interpreter.Get(), nullptr);
        }
    private:
        FunctionStatementAST* function_;
    };
    
    static Value Print(const std::vector<Value>& args) {
        std::vector<Value>::const_iterator iter = args.begin();
        for(; iter != args.end(); ++iter) {
            switch (iter->type_) {
                case Value_Number:
                    printf("%lf\n", iter->number_);
                    break;
                case Value_Boolean:
                    printf("%s\n", iter->boolean_ ? "true" : "false");
                    break;
                default:
                    break;
            }
        }
        return Value();
    }
    
    void Interpreter::Initialize() {
        Value value;
        value.native_function_ = reinterpret_cast<void*>(Print);
        value.type_ = ValueT_CFunction;
        variables_map_.insert(std::make_pair(string_pool_->NewString("print"), value));
//
//        Value table;
//        table.type_ = Value_Table;
//        Dictonary* dict = new Dictonary;
//        table.table_ = dict;
//        Value target;
//        Dictonary* dict1 = new Dictonary;
//        target.type_ = Value_Table;
//        target.table_ = dict1;
//        dict1->SetValue(string_pool_->NewString("scrollTop"), Value(100.0));
//        dict->SetValue(string_pool_->NewString("target"), target);
//        variables_map_.insert(std::make_pair(string_pool_->NewString("e"), table));
    }
    
    void Interpreter::SearchVariable(const String* name, ValueData* value) {
        Interpreter* interpreter = this;
        while(interpreter != nullptr) {
            auto iter = interpreter->variables_map_.find(const_cast<String*>(name));
            if(iter != interpreter->variables_map_.end()) {
                value->value_ = iter->second;
                return;
            }
            interpreter = interpreter->parent_;
        }
    }
    
    void Interpreter::AssignVariable(const String* name, ValueData* value) {
        Interpreter* interpreter = this;
        while(interpreter != nullptr) {
            auto iter = interpreter->variables_map_.find(const_cast<String*>(name));
            if(iter != interpreter->variables_map_.end()) {
                switch (value->assigment_) {
                    case Token_ASSIGN_ADD:
                        iter->second += value->value_;
                        break;
                    case Token_ASSIGN_SUB:
                        iter->second -= value->value_;
                        break;
                    case Token_ASSIGN_MUL:
                        iter->second *= value->value_;
                        break;
                    case Token_ASSIGN_DIV:
                        iter->second /= value->value_;
                        break;
                    case Token_ASSIGN_MOD:
                        iter->second %= value->value_;
                        break;
//                    case Token_ASSIGN_BIT_OR:
//                        iter->second |= value->value_;
//                        break;
//                    case Token_ASSIGN_BIT_AND:
//                        iter->second &= value->value_;
//                        break;
//                    case Token_ASSIGN_BIT_XOR:
//                        iter->second ^= value->value_;
//                        break;
                    default:
                        iter->second = value->value_;
                        break;
                }
                return;
            }
            interpreter = interpreter->parent_;
        }
    }
    
    void Interpreter::Call(const String* name, const std::vector<Value>& args) {
        std::unordered_map<String*, Value>::iterator iter = variables_map_.find(const_cast<String*>(name));
        if(iter != variables_map_.end()) {
            iter->second.closure_->Run(this, args);
        }
    }
    
    void Interpreter::Visit(ChunkAST* ast, void* data) {
        ast->block()->Accept(this, nullptr);
    }
    
    void Interpreter::Visit(BlockAST* ast, void* data) {
        for(base::ScopedVector<ASTree>::iterator iter = ast->statements().begin();
            iter != ast->statements().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
        if(ast->return_statement().Get()) {
            ast->return_statement()->Accept(this, nullptr);
        }
    }
    
    void Interpreter::Visit(ReturnStatementAST* ast, void* data){
        
    }
    
    void Interpreter::Visit(LiteralAST* ast, void* data){
        ValueData* value_data = static_cast<ValueData*>(data);
        Value* value = &(value_data->value_);
        switch (ast->token().token_) {
            case Token_Nil:
                value->type_ = Value_Nil;
                break;
            case Token_Id:
                if(value_data->op_ == ValueOp_Read) {
                    SearchVariable(ast->token().str_, value_data);
                }else{
                    AssignVariable(ast->token().str_, value_data);
                }
                break;
            case Token_Number:
                value->type_ = Value_Number;
                value->number_ = ast->token().number_;
                break;
            case Token_String:
                value->type_ = Value_Nil;
                break;
            case Token_False:
                value->type_ = Value_Boolean;
                value->boolean_ = false;
                break;
            case Token_True:
                value->type_ = Value_Boolean;
                value->boolean_ = true;
                break;
            default:
                break;
        }
    }
    
    void Interpreter::Visit(NamesAST* ast, void* data){
        std::vector<Value>* args = static_cast<std::vector<Value>*>(data);
        std::vector<Token>::iterator iter_name = ast->names().begin();
        std::vector<Value>::iterator iter_arg = args->begin();
        for(;iter_name != ast->names().end() && iter_arg != args->end(); ++iter_name,++iter_arg) {
            variables_map_.insert(std::make_pair((*iter_name).str_, *iter_arg));
        }
    }
    
    void Interpreter::Visit(BinaryExprAST* ast, void* data){
        ValueData l_value, r_value;
        ast->left()->Accept(this, &l_value);
        ast->right()->Accept(this, &r_value);
        
        ValueData* value_data = static_cast<ValueData*>(data);
        Value* value = &(value_data->value_);
        
        switch (ast->op_token().token_) {
            case '+':
                *value = l_value.value_ + r_value.value_;
                break;
            case '-':
                *value = l_value.value_ - r_value.value_;
                break;
            case '*':
                *value = l_value.value_ * r_value.value_;
                break;
            case '/':
                *value = l_value.value_ / r_value.value_;
                break;
            case '%':
                *value = l_value.value_ % r_value.value_;
                break;
            case '<':
                value->boolean_ = l_value.value_.number_ < r_value.value_.number_;
                value->type_ = Value_Boolean;
                break;
            case '>':
                value->boolean_ = l_value.value_.number_ > r_value.value_.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_LessEqual:
                value->boolean_ = l_value.value_.number_ <= r_value.value_.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_GreaterEqual:
                value->boolean_ = l_value.value_.number_ >= r_value.value_.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_NotEqual:
                value->boolean_ = l_value.value_ != r_value.value_;
                value->type_ = Value_Boolean;
                break;
            case Token_Equal:
                value->boolean_ = l_value.value_ == r_value.value_;
                value->type_ = Value_Boolean;
                break;
            case Token_And:
                if(l_value.value_.type_ == Value_Number && r_value.value_.type_ == Value_Number) {
                    value->boolean_ = l_value.value_.number_ && r_value.value_.number_;
                }else if(l_value.value_.type_ == Value_Boolean && r_value.value_.type_ == Value_Boolean) {
                    value->boolean_ = l_value.value_.boolean_ && r_value.value_.boolean_;
                }
                value->type_ = Value_Boolean;
                break;
            case Token_Or:
                if(l_value.value_.type_ == Value_Number && r_value.value_.type_ == Value_Number) {
                    value->boolean_ = l_value.value_.number_ || r_value.value_.number_;
                }else if(l_value.value_.type_ == Value_Boolean && r_value.value_.type_ == Value_Boolean) {
                    value->boolean_ = l_value.value_.boolean_ || r_value.value_.boolean_;
                }
                value->type_ = Value_Boolean;
                break;
            default:
                *value = r_value.value_;
                break;
        }
    }
    
    void Interpreter::Visit(UnaryExpression* ast, void* data){
        ValueData* value_data = static_cast<ValueData*>(data);
        Value* value = &(value_data->value_);
        ValueData unary_value_data;
        ast->expression()->Accept(this, &unary_value_data);
        switch (ast->op_token().token_) {
            case '-':
                value->number_ = -unary_value_data.value_.number_;
                value->type_ = Value_Number;
                break;
            case '!':
                value->boolean_ = !unary_value_data.value_.boolean_;
                value->type_ = Value_Boolean;
                break;
            default:
                break;
        }
    }
    
    void Interpreter::Visit(ExpressionListAST* ast, void* data){
        std::vector<Value>* args = static_cast<std::vector<Value>*>(data);
        for(base::ScopedVector<ASTree>::iterator iter = ast->expressions().begin();
            iter != ast->expressions().end(); ++iter) {
            if(*iter) {
                ValueData value_data;
                (*iter)->Accept(this, &value_data);
                args->push_back(value_data.value_);
            }
        }
    }
    
    void Interpreter::Visit(VariableAST* ast, void* data){
        ValueData value_data;
        ast->expression()->Accept(this, &value_data);
        if(ast->identifier().token_ != Token_Id) {
            //TODO exception
        }
        variables_map_.insert(std::make_pair(ast->identifier().str_, value_data.value_));
    }
    
    void Interpreter::Visit(VariableListAST* ast, void* data){
        for(base::ScopedVector<VariableAST>::iterator iter = ast->variable_list().begin();
            iter != ast->variable_list().end(); ++iter) {
            (*iter)->Accept(this, nullptr);
        }
    }
    
    void Interpreter::Visit(FunctionStatementAST* ast, void* data){
        Value value;
        value.closure_ = new Closure;
        value.type_ = ValueT_Closure;
        value.closure_->SetFunction(ast);
        variables_map_.insert(std::make_pair(ast->function_name().str_, value));
    }
    
    void Interpreter::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void Interpreter::Visit(WhileStatementAST* ast, void* data){
        
    }
    
    void Interpreter::Visit(IfStatementAST* ast, void* data){
        ValueData value_data;
        ast->condition()->Accept(this, &value_data);
        if(value_data.value_.type_ == Value_Boolean) {
            if(value_data.value_.boolean_) {
                ast->true_branch()->Accept(this, nullptr);
            }else if(ast->false_branch().Get()) {
                ast->false_branch()->Accept(this, nullptr);
            }
        }else if(value_data.value_.type_ == Value_Number) {
            if(value_data.value_.number_) {
                ast->true_branch()->Accept(this, nullptr);
            }else if(ast->false_branch().Get()) {
                ast->false_branch()->Accept(this, nullptr);
            }
        }
    }
    
    void Interpreter::Visit(ElseStatementAST* ast, void* data){
        ast->block()->Accept(this, nullptr);
    }
    
    void Interpreter::Visit(AssignStatement* ast, void* data){
        ValueData value_data;
        ast->expression()->Accept(this, &value_data);
        value_data.op_ = ValueOp_Write;
        value_data.assigment_ = ast->assignment().token_;
        ast->variable()->Accept(this, &value_data);
        
    }
    
    void Interpreter::Visit(MemberAccessorAST* ast, void* data){
        ValueData* value_data = static_cast<ValueData*>(data);
        if(value_data->op_ == ValueOp_Write) {
            Value table;
            ast->table()->Accept(this, &table);
            if(table.type_ == Value_Table) {
                static_cast<Dictonary*>(table.table_)->SetValue(ast->member().str_, value_data->value_);
            }
        }else{
             ast->table()->Accept(this, value_data);
            if(value_data->value_.type_ == Value_Table) {
                value_data->value_ = static_cast<Dictonary*>(value_data->value_.table_)->GetValue(ast->member().str_);
            }
        }
    }
    
    void Interpreter::Visit(FunctionCallAST* ast, void* data) {
        ValueData value_data;
        ast->caller()->Accept(this, &value_data);
        std::vector<Value> args;
        ast->args()->Accept(this, &args);
        if(value_data.value_.type_ == ValueT_Closure) {
            value_data.value_.closure_->Run(this, args);
        }else if(value_data.value_.type_ == ValueT_CFunction){
            reinterpret_cast<NativeFunction>(value_data.value_.native_function_)(args);
        }else {
            // TODO
        }
    }
}
