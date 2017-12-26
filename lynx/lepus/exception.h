#ifndef LYNX_LEPUS_EXCEPTION_H_
#define LYNX_LEPUS_EXCEPTION_H_

#include <string>
#include <sstream>

namespace lepus {
    class Exception {
    public:
        const std::string message() const {
            return stream_.str();
        }
        
    protected:
        std::ostringstream& stream() {
            return stream_;
        }
    private:
        std::ostringstream stream_;
    };
    
    class CompileException : public Exception {
    public:
        CompileException(const char* msg, const Token& token) {
            stream()<<"(line:"<<token.line_
            <<", column:"<<token.column_<<"):"<<msg;
        }
        
        CompileException(const char* key, const char* msg, const Token& token) {
            stream()<<"(line:"<<token.line_
            <<", column:"<<token.column_<<"):"<<key<<msg;
        }
    };
    class RuntimeException : public Exception {
    public:
        RuntimeException(const char* msg) {
            stream()<<msg;
        }
        RuntimeException(const char* tag ,const char* msg) {
            stream()<<tag<<msg;
        }
    };
}

#endif  // LYNX_LEPUS_EXCEPTION_H_
