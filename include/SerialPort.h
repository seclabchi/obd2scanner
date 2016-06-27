#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <string>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

using namespace std;

class SerialPort
{
    public:
    SerialPort(string portName);
    virtual ~SerialPort();
    void open();
    int close();
    bool isOpen();
    
    private:
    string m_portName;
    int m_portFd;
    bool m_isOpen;
};

#endif //__SERIALPORT_H__