#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdarg.h>
#include <string>
#include <string.h>

#include "Exception.h"

using namespace std;

namespace Tonekids
{
    typedef enum
    {
        ALL = 0,
        DEBUG,
        INFO,
        WARNING,
        EXCEPTION,
        ERROR,
        CRITICAL,
        NONE
    } LOG_LEVEL;
    
    void logMsg(LOG_LEVEL level, const char* file, int line, const char* format, ...);
    void logException(Exception e);
    
    class Logger
    {
    public:
        static Logger* getInstance();
        void logMsg(LOG_LEVEL level, const char* file, int line, const char* format, va_list args);
        void logException(Exception e);
        string logLevelToString(LOG_LEVEL level);
        string getFileString(const char* rawFileStr);
    private:
        static Logger* m_instance;
    };
}

#define LOGD(...) Tonekids::logMsg(Tonekids::DEBUG, __FILE__, __LINE__, __VA_ARGS__);
#define LOGI(...) Tonekids::logMsg(Tonekids::INFO, __FILE__, __LINE__, __VA_ARGS__);
#define LOGW(...) Tonekids::logMsg(Tonekids::WARNING, __FILE__, __LINE__, __VA_ARGS__);
#define LOGE(...) Tonekids::logMsg(Tonekids::ERROR, __FILE__, __LINE__, __VA_ARGS__);
#define LOGC(...) Tonekids::logMsg(Tonekids::CRITICAL, __FILE__, __LINE__, __VA_ARGS__);
#define LOG_EXCEPTION(e) Tonekids::logException(e);


#endif //__LOGGER_H__ 