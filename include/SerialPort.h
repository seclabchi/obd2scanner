#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#include <string>
#include <vector>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>

#define RX_BUF_SIZE 4096

using namespace std;

class SerialPort
{
    public:
    SerialPort(string portName, uint32_t baudRate);
    virtual ~SerialPort();
    void open();
    int close();
    bool isOpen();
    void setLineDelimiter(char delim);
    
    void txData(string msg, bool useDelim = true);
    void txData(const char* msg, bool useDelim = true);
    void txData(vector<uint8_t> msg, bool useDelim = true);
    vector<uint8_t> rxData();
    vector<uint8_t> readLine();

    private:
    string m_portName;
    uint32_t m_baudRate;
    int m_portFd;
    bool m_isOpen;
    struct termios m_existingPortSettings;
    struct termios m_currentPortSettings;
    uint8_t m_rxBuf[RX_BUF_SIZE];
    char m_lineDelim;
};

#endif //__SERIALPORT_H__