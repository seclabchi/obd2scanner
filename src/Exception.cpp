#include "Exception.h"

namespace Tonekids
{
    #define MAX_MSG_LEN 4096
    
    void throwException(const char* file, int line, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        Exception e(file, line, format, args);
        va_end(args);
        throw e;
    }
    
    Exception::Exception(const char* file, int line, const char* format, va_list args)
    {
        char exceptionMsg[MAX_MSG_LEN];
        m_file = string(file);
        m_line = line;
        
        vsnprintf(exceptionMsg, MAX_MSG_LEN, format, args);

        m_msg = string(exceptionMsg);
    }
    
    Exception::~Exception()
    {
        
    }
    
    string Exception::getMsg()
    {
        return m_msg;
    }
    
    string Exception::getFile()
    {
        return m_file;
    }
    
    int Exception::getLine()
    {
        return m_line;
    }
}