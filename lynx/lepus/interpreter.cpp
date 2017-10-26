
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
    
    typedef Value (*NativeFunction)(const std::vector<Value>& args);
    
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
        variables_map_.insert(std::make_pair("print", value));
        
        Value table;
        table.type_ = Value_Table;
        Dictonary* dict = new Dictonary;
        table.table_ = dict;
        Value target;
        Dictonary* dict1 = new Dictonary;
        target.type_ = Value_Table;
        target.table_ = dict1;
        dict1->SetValue("scrollTop", Value(100.0));
        dict->SetValue("target", target);
        variables_map_.insert(std::make_pair("e", table));
    }
    
    void Interpreter::SearchVariable(const std::string& name, Value* value) {
        Interpreter* interpreter = this;
        while(interpreter != nullptr) {
            auto iter = interpreter->variables_map_.find(name);
            if(iter != interpreter->variables_map_.end()) {
                *value = iter->second;
                return;
            }
            interpreter = interpreter->parent_;
        }
    }
    
    void Interpreter::AssignVariable(const std::string& name, Value* value) {
        Interpreter* interpreter = this;
        while(interpreter != nullptr) {
            auto iter = interpreter->variables_map_.find(name);
            if(iter != interpreter->variables_map_.end()) {
                iter->second = *value;
                return;
            }
            interpreter = interpreter->parent_;
        }
    }
    
    void Interpreter::Call(const std::string& name, const std::vector<Value>& args) {
        std::unordered_map<std::string, Value>::iterator iter = variables_map_.find(name);
        if(iter != variables_map_.end()) {
            static_cast<Closure*>(iter->second.closure_)->Run(this, args);
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
        Value* value = static_cast<Value*>(data);
        switch (ast->token().token_) {
            case Token_Nil:
                value->type_ = Value_Nil;
                break;
            case Token_Id:
                if(value->type_ == Value_Nil) {
                    SearchVariable(*ast->token().str_, value);
                }else{
                    AssignVariable(*ast->token().str_, value);
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
            variables_map_.insert(std::make_pair(*(*iter_name).str_, *iter_arg));
        }
    }
    
    void Interpreter::Visit(BinaryExprAST* ast, void* data){
        Value l_value, r_value;
        ast->left()->Accept(this, &l_value);
        ast->right()->Accept(this, &r_value);
        
        Value* value = static_cast<Value*>(data);
        
        switch (ast->op_token().token_) {
            case '+':
                value->number_ = l_value.number_ + r_value.number_;
                value->type_ = Value_Number;
                break;
            case '-':
                value->number_ = l_value.number_ - r_value.number_;
                value->type_ = Value_Number;
                break;
            case '*':
                value->number_ = l_value.number_ * r_value.number_;
                value->type_ = Value_Number;
                break;
            case '/':
                value->number_ = l_value.number_ / r_value.number_;
                value->type_ = Value_Number;
                break;
            case '%':
                value->number_ = ((int)l_value.number_)% ((int)r_value.number_);
                value->type_ = Value_Number;
                break;
            case '<':
                value->boolean_ = l_value.number_ < r_value.number_;
                value->type_ = Value_Boolean;
                break;
            case '>':
                value->boolean_ = l_value.number_ > r_value.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_LessEqual:
                value->boolean_ = l_value.number_ <= r_value.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_GreaterEqual:
                value->boolean_ = l_value.number_ >= r_value.number_;
                value->type_ = Value_Boolean;
                break;
            case Token_NotEqual:
                if(l_value.type_ == Value_Number && r_value.type_ == Value_Number) {
                    value->boolean_ = l_value.number_ != r_value.number_;
                }else if(l_value.type_ == Value_Boolean && r_value.type_ == Value_Boolean) {
                    value->boolean_ = l_value.boolean_ != r_value.boolean_;
                }
                value->type_ = Value_Boolean;
            case Token_Equal:
                if(l_value.type_ == Value_Number && r_value.type_ == Value_Number) {
                    value->boolean_ = l_value.number_ == r_value.number_;
                }else if(l_value.type_ == Value_Boolean && r_value.type_ == Value_Boolean) {
                    value->boolean_ = l_value.boolean_ == r_value.boolean_;
                }
                value->type_ = Value_Boolean;
            default:
                break;
        }
    }
    
    void Interpreter::Visit(UnaryExpression* ast, void* data){
        
    }
    
    void Interpreter::Visit(ExpressionListAST* ast, void* data){
        std::vector<Value>* args = static_cast<std::vector<Value>*>(data);
        for(base::ScopedVector<ASTree>::iterator iter = ast->expressions().begin();
            iter != ast->expressions().end(); ++iter) {
            if(*iter) {
                Value value;
                (*iter)->Accept(this, &value);
                args->push_back(value);
            }
        }
    }
    
    void Interpreter::Visit(VariableAST* ast, void* data){
        Value value;
        ast->expression()->Accept(this, &value);
        if(ast->identifier().token_ != Token_Id) {
            //TODO exception
        }
        variables_map_.insert(std::make_pair(*(ast->identifier().str_), value));
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
        static_cast<Closure*>(value.closure_)->SetFunction(ast);
        variables_map_.insert(std::make_pair(*(ast->function_name().str_), value));
    }
    
    void Interpreter::Visit(BreakStatementAST* ast, void* data){
        
    }
    
    void Interpreter::Visit(WhileStatementAST* ast, void* data){
        
    }
    
    void Interpreter::Visit(IfStatementAST* ast, void* data){
        Value value;
        ast->condition()->Accept(this, &value);
        if(value.type_ == Value_Boolean) {
            if(value.boolean_) {
                ast->true_branch()->Accept(this, nullptr);
            }else if(ast->false_branch().Get()) {
                ast->false_branch()->Accept(this, nullptr);
            }
        }else if(value.type_ == Value_Number) {
            if(value.number_) {
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
        Value value;
        ast->expression()->Accept(this, &value);
        ast->variable()->Accept(this, &value);
    }
    
    void Interpreter::Visit(MemberAccessorAST* ast, void* data){
        Value* value = static_cast<Value*>(data);
        if(value->type_ != Value_Nil) {
            Value table;
            ast->table()->Accept(this, &table);
            if(table.type_ == Value_Table) {
                static_cast<Dictonary*>(table.table_)->SetValue(*ast->member().str_, *value);
            }
        }else{
             ast->table()->Accept(this, value);
            if(value->type_ == Value_Table) {
                *value = static_cast<Dictonary*>(value->table_)->GetValue(*ast->member().str_);
            }
        }
    }
    
    void Interpreter::Visit(FunctionCallAST* ast, void* data) {
        Value value;
        ast->caller()->Accept(this, &value);
        std::vector<Value> args;
        ast->args()->Accept(this, &args);
        if(value.type_ == ValueT_Closure) {
            static_cast<Closure*>(value.closure_)->Run(this, args);
        }else if(value.type_ == ValueT_CFunction){
            reinterpret_cast<NativeFunction>(value.native_function_)(args);
        }
    }
}
