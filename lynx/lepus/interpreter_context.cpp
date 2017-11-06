
#include "lepus/interpreter_context.h"
#include "parser/input_stream.h"
#include "lepus/scanner.h"
#include "lepus/parser.h"
#include "lepus/exception.h"
#include "lepus/semantic_analysis.h"
#include "lepus/interpreter.h"
#include "lepus/value.h"

namespace lepus {
    void InterpreterContext::Execute(const std::string& source) {
        parser::InputStream input;
        input.Write(source);
        Scanner scanner(&input, &string_pool_);
        Parser parser(&scanner);
        interpreter_.Initialize();
        //SemanticAnalysis semantic_analysis;
        ASTree* root = nullptr;
        try {
            root = parser.Parse();
            root->Accept(&interpreter_, nullptr);
        }catch(const lepus::Exception& exception) {
            std::cout<<exception.message()<<std::endl;
        }
        root_.Reset(root);
    }
    
    Value InterpreterContext::Call(const std::string& name, const std::vector<Value>& args) {
        interpreter_.Call(string_pool_.NewString(name), args);
        return Value();
    }
}
