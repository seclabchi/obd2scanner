#ifndef __COMMINTERFACE_H__
#define __COMMINTERFACE_H__

#include <string>
#include <vector>
#include <stdint.h>

#include "Exception.h"
#include "Logger.h"
#include "SerialPort.h"

using namespace std;
using namespace Tonekids;

class CommInterface
{
public:
    CommInterface(string portName, uint32_t baudRate);
    virtual ~CommInterface();
    
    vector<string> reset();
    
private:
    vector<uint8_t> performTransaction(const char* txMsg, uint8_t postTxWait = 0, uint8_t linesExpected = 1);
    SerialPort* m_serialPort;
};

#endif //__COMMINTERFACE_H__