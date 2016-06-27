#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdio.h>
#include <stdarg.h>
#include <string>

using namespace std;

namespace Tonekids
{
    void throwException(const char* file, int line, const char* format, ...);
    
    class Exception
    {
    public:
        Exception(const char* file, int line, const char* format, va_list args);
        virtual ~Exception();
        string getMsg();
        string getFile();
        int getLine();
    private:
        string m_msg;
        string m_file;
        int m_line;
    };
}

#define THROW_EXCEPTION(...) Tonekids::throwException(__FILE__, __LINE__, __VA_ARGS__);



#endif //__EXCEPTION_H__