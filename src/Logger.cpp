#include "Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#define MAX_MSG_LEN 4096

using namespace std;

namespace Tonekids
{    
    Logger* s_instance = NULL;
    
    Logger* Logger::getInstance()
    {
        if(NULL == s_instance)
        {
            s_instance = new Logger();
        }
        
        return s_instance;
    }
    
    void logMsg(LOG_LEVEL level, const char* file, int line, const char* format, ...)
    {
        Logger* log = Logger::getInstance();
        
        va_list args;
        va_start(args, format);
        log->logMsg(level, file, line, format, args);
        va_end(args);
    }
    
    void logException(Exception e)
    {
        Logger* log = Logger::getInstance();
        
        log->logException(e);
    }

    
    void Logger::logMsg(LOG_LEVEL level, const char* file, int line, const char* format, va_list args)
    {
        ostringstream os;
        
        char logMsg[MAX_MSG_LEN];
        vsnprintf(logMsg, MAX_MSG_LEN, format, args);
        
        os << logLevelToString(level) << "\t" << getFileString(file) << ":" << line << " - " << logMsg;
        cout << os.str() << endl;
    }
    
    void Logger::logException(Exception e)
    {
        ostringstream os;
        os << logLevelToString(EXCEPTION) << "\t" << e.getFile() << ":" << e.getLine() << " - "<< e.getMsg();
        cout << os.str() << endl;
    }
    
    string Logger::getFileString(const char* rawFileStr)
    {
        const char* trimmed = strrchr(rawFileStr, '/');
        trimmed++;
        return trimmed;
    }

    string Logger::logLevelToString(LOG_LEVEL level)
    {
        string levelStr;
        
        switch(level)
        {
            case ALL:
            levelStr = "ALL";
            break;
            case DEBUG:
            levelStr = "DEBUG";
            break;
            case INFO:
            levelStr = "INFO";
            break;
            case WARNING:
            levelStr = "WARNING";
            break;
            case EXCEPTION:
            levelStr = "EXCPT";
            break;
            case ERROR:
            levelStr = "ERROR";
            break;
            case CRITICAL:
            levelStr = "CRITICAL";
            break;
            case NONE:
            levelStr = "NONE";
            break;
        }
        
        return levelStr;
    }
}