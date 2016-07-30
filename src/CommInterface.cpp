#include "CommInterface.h"

#include <iostream>

CommInterface::CommInterface(string portName, uint32_t baudRate)
{
    try
    {
        m_serialPort = new SerialPort(portName, baudRate);
        m_serialPort->setLineDelimiter('\r');
        m_serialPort->open();
    }
    catch(Exception e)
    {
        LOG_EXCEPTION(e);
        m_serialPort = NULL;
    }
}

vector<uint8_t> CommInterface::performTransaction(const char* txMsg, uint8_t postTxWait, uint8_t linesExpected)
{
	uint8_t lines_received = 0;
	
    if(NULL == m_serialPort)
    {
        THROW_EXCEPTION("Serial port is NULL.");
    }
        
    m_serialPort->txData(txMsg);
    
    sleep(postTxWait);
    
    vector<uint8_t> rxMsg;
    
    do
    {
        rxMsg = m_serialPort->readLine();

        if(rxMsg.size() > 0)
        {
          string rxMsgStr(rxMsg.begin(), rxMsg.end());
          cout << "received " << rxMsg.size() << " bytes: " << rxMsgStr << endl;
          rxMsgStr.append("\n");
		  linesReceived++;
        }
    } while(rxMsg.size() > 0);
    
    return rxMsg;
}

vector<string> CommInterface::reset()
{
    vector<string> totalResponseData;
    vector<uint8_t> responseData;
    
    responseData = this->performTransaction("AT Z");
    totalResponseData.push_back(string(responseData.begin(), responseData.end()));
    
    responseData = this->performTransaction("AT SP 0");
    totalResponseData.push_back(string(responseData.begin(), responseData.end()));
    
    responseData = this->performTransaction("01 00", 3); //wait three seconds for the vehicle
                                            //to collect all of the data
    totalResponseData.push_back(string(responseData.begin(), responseData.end()));
    
    return totalResponseData;
}

CommInterface::~CommInterface()
{
    if(NULL != m_serialPort)
    {
        m_serialPort->close();
        delete m_serialPort;
        m_serialPort = NULL;
    }
}