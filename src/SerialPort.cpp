#include "SerialPort.h"
#include "Exception.h"

#define BAUDRATE B19200

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
    
    m_portFd = ::open(m_portName.c_str(), O_RDWR | O_NOCTTY);
        
    if(m_portFd < 0)
    {
        m_portFd = -1;
        THROW_EXCEPTION("Failed to open serial port with error: %d - %s", 
                        errno, strerror(errno));
    }
    
    tcgetattr(m_portFd, &m_existingPortSettings);
    memset(&m_currentPortSettings, 0, sizeof(struct termios));
    
    /* 
      BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
      CS8     : 8n1 (8bit,no parity,1 stopbit)
      CLOCAL  : local connection, no modem contol
      CREAD   : enable receiving characters
    */
     m_currentPortSettings.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
     
    /*
      IGNPAR  : ignore bytes with parity errors
      ICRNL   : map CR to NL (otherwise a CR input on the other computer
                will not terminate input)
      otherwise make device raw (no other input processing)
    */
     m_currentPortSettings.c_iflag = IGNPAR;
     
    /*
     Raw output.
    */
     m_currentPortSettings.c_oflag = 0;
     
    /*
      ICANON  : enable canonical input
      disable all echo functionality, and don't send signals to calling program
    */
     //m_currentPortSettings.c_lflag = ~ICANON;
     
    /* 
      initialize timeouts
      default values can be found in /usr/include/termios.h, and are given
      in the comments, but we don't need them here
    */
     m_currentPortSettings.c_cc[VTIME]    = 2;     /* inter-character timer 0.2s */
     m_currentPortSettings.c_cc[VMIN]     = 0;     /* timed read based on VTIME */
    
    /* 
      now clean the modem line and activate the settings for the port
    */
     tcflush(m_portFd, TCIFLUSH);
     tcsetattr(m_portFd, TCSANOW, &m_currentPortSettings);
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

void SerialPort::setLineDelimiter(char delim)
{
    m_lineDelim = delim;
}

    
void SerialPort::txData(vector<uint8_t> msg)
{
    if(m_portFd < 0)
    {
        THROW_EXCEPTION("Serial port is not open.");
    }
    
    ssize_t bytesWritten = 0;
    
    while(bytesWritten < (ssize_t)msg.size())
    {
        bytesWritten += write(m_portFd, &msg[bytesWritten], msg.size() - bytesWritten);
        if(-1 == bytesWritten)
        {
            THROW_EXCEPTION("Failed writing to serial port with errno %d: %s", errno, strerror(errno));
        }
    }
}

vector<uint8_t> SerialPort::rxData()
{
    ssize_t bytesRead = 0;
    ssize_t totalBytesRead = 0;
    
    do
    {
        bytesRead = read(m_portFd, m_rxBuf + totalBytesRead, RX_BUF_SIZE - totalBytesRead);
        totalBytesRead += bytesRead;
        
        if(-1 == bytesRead)
        {
            THROW_EXCEPTION("Failed reading from serial port with errno %d: %s", errno, strerror(errno));
        }
    } while(bytesRead > 0);
    
    
    vector<uint8_t> rxData(m_rxBuf, m_rxBuf + totalBytesRead);
    
    return rxData;
}