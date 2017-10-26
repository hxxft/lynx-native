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
    
    class ParseException : public Exception {
    public:
        ParseException(const char* msg, const Token& token) {
            stream()<<token.module_<<"(line:"<<token.line_
            <<", column:"<<token.column_<<"):"<<msg;
        }
        
    };
    
    class SemantException : public Exception {
    public:
        SemantException(const char* msg, const Token& token) {
            stream()<<token.module_<<"(line:"<<token.line_
            <<", column:"<<token.column_<<"):"<<msg;
        }
        
    };
}

#endif  // LYNX_LEPUS_EXCEPTION_H_
