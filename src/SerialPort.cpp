#include "SerialPort.h"
#include "Exception.h"
#include "Logger.h"

SerialPort::SerialPort(string portName, uint32_t baudRate) : m_portName(portName) 
{
    m_portFd = -1;
	m_lineDelim = '\0';
    
    switch(baudRate)
    {
        case 50:
            m_baudRate = B50;
            break;
        case 75:
            m_baudRate = B75;
            break;
        case 110:
            m_baudRate = B110;
            break;
        case 134:
            m_baudRate = B134;
            break;
        case 150:
            m_baudRate = B150;
            break;
        case 200:
            m_baudRate = B200;
            break;
        case 300:
            m_baudRate = B300;
            break;
        case 600:
            m_baudRate = B600;
            break;
        case 1200:
            m_baudRate = B1200;
            break;
        case 1800:
            m_baudRate = B1800;
            break;
        case 2400:
            m_baudRate = B2400;
            break;
        case 4800:
            m_baudRate = B4800;
            break;
        case 9600:
            m_baudRate = B9600;
            break;
        case 19200:
            m_baudRate = B19200;
            break;
        case 38400:
            m_baudRate = B38400;
            break;
        case 57600:
            m_baudRate = B57600;
            break;
        case 115200:
            m_baudRate = B115200;
            break;
        case 230400:
            m_baudRate = B230400;
            break;
        case 460800:
            m_baudRate = B460800;
            break;
        case 500000:
            m_baudRate = B500000;
            break;
        case 576000:
            m_baudRate = B576000;
            break;
        case 921600:
            m_baudRate = B921600;
            break;
        case 1000000:
            m_baudRate = B1000000;
            break;
        case 1152000:
            m_baudRate = B1152000;
            break;
        case 1500000:
            m_baudRate = B1500000;
            break;
        case 2000000:
            m_baudRate = B2000000;
            break;
        case 2500000:
            m_baudRate = B2500000;
            break;
        case 3000000:
            m_baudRate = B3000000;
            break;
        case 3500000:
            m_baudRate = B3500000;
            break;
        case 4000000:
            m_baudRate = B4000000;
            break;
        default:
            m_baudRate = B38400;
            LOGW("Invalid baud rate %d selected, defaulting to %d.", baudRate, m_baudRate);
            break;
    }
}

SerialPort::~SerialPort()
{
    
}

void SerialPort::open(uint32_t charWait)
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
     m_currentPortSettings.c_cflag = m_baudRate | CS8 | CLOCAL | CREAD;
     
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
     m_currentPortSettings.c_cc[VTIME]    = charWait;     /* inter-character timer 0.2s */
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

void SerialPort::txData(string msg, bool useDelim)
{
    vector<uint8_t> rawMsg(msg.begin(), msg.end());
    this->txData(rawMsg, useDelim);
}

void SerialPort::txData(const char* msg, bool useDelim)
{
    this->txData(string(msg), useDelim);
}
    
void SerialPort::txData(vector<uint8_t> msg, bool useDelim)
{
    if(useDelim)
    {
        msg.push_back((uint8_t)m_lineDelim);
    }
    
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

/* This will read all bytes available with the configured timeout on the serial
 * port.  Future enhancements should include a function which will read a specified
 * number of bytes to avoid a timeout wait.
 */
vector<uint8_t> SerialPort::rxData(uint32_t numChars)
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
	
	if(bytesRead < numChars)
	{
		THROW_EXCEPTION("Read %d bytes: less than required number of %d bytes.", numChars, bytesRead);
	}
    
    vector<uint8_t> rxData(m_rxBuf, m_rxBuf + totalBytesRead);
    
    return rxData;
}

vector<uint8_t> SerialPort::readLine()
{
    ssize_t bytesRead = 0;
    ssize_t totalBytesRead = 0;
    bool foundDelim = false;
    vector<uint8_t> rxData;
    
    do
    {
        bytesRead = read(m_portFd, m_rxBuf + totalBytesRead, 1);
        
        if(bytesRead > 0)
        {
            if(m_rxBuf[totalBytesRead] == m_lineDelim)
            {
                foundDelim = true;
            }
            
            totalBytesRead ++;
        }
        
        if(-1 == bytesRead)
        {
            THROW_EXCEPTION("Failed reading from serial port with errno %d: %s", errno, strerror(errno));
        }
    } while((false == foundDelim) && (bytesRead != 0));
    
    if((false == foundDelim) && (bytesRead > 0))
    {
        THROW_EXCEPTION("Timed out waiting for end-of-line delimiter.");
    }
    
    if(0 < totalBytesRead)
    {
        rxData = vector<uint8_t>(m_rxBuf, m_rxBuf + totalBytesRead);
    }
    
    return rxData;
}