#include "SerialPort.h"
#include "Exception.h"

SerialPort::SerialPort(string portName) : m_portName(portName)
{
    m_portFd = -1;
}

SerialPort::~SerialPort()
{
    
}

void SerialPort::open()
{
    if(0 <= m_portFd)
    {
        THROW_EXCEPTION("Serial port is already open.");
    }
    
    m_portFd = ::open(m_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    
    if(m_portFd < 0)
    {
        m_portFd = -1;
        THROW_EXCEPTION("Failed to open serial port with error: %d - %s", 
                        errno, strerror(errno));
    }
}

int SerialPort::close()
{
    int retVal = 0;
    
    retVal = ::close(m_portFd);
    m_portFd = -1;
    
    return retVal;
}

bool SerialPort::isOpen()
{
    bool open = false;
    
    if(m_portFd > -1)
    {
        open = true;
    }
    
    return open;
}
    
    