#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include <iostream>
#include <vector>

#include "Logger.h"
#include "Exception.h"

#include "SerialPort.h"

using namespace Tonekids;
using namespace std;

int main(int argc, char **argv) 
{
  LOGI("Hello and welcome to the OBD2 Scanner.");
  
  Fl_Window *window = new Fl_Window(320,240);
  Fl_Box *title_box = new Fl_Box(5,5,300,20,"OBD-II SCANNER");
  title_box->box(FL_NO_BOX);
  title_box->labelfont(FL_BOLD);
  title_box->labelsize(14);
  
  window->fullscreen();
  
  int X, Y, W, H;
  
  int& Xref = X;
  int& Yref = Y;
  int& Wref = W;
  int& Href = H;
  
  Fl::screen_work_area(Xref, Yref, Wref, Href);
  
  LOGD("Screen work area: %d, %d, %d, %d", X, Y, W, H);
  
  Fl::screen_xywh(Xref, Yref, Wref, Href);
  
  LOGD("Screen xywh: %d, %d, %d, %d", X, Y, W, H);
  
  window->border(0);

  window->end();
  window->show(argc, argv);
  
  SerialPort* sp = 0;
  
  try
  {
      sp = new SerialPort(string("/dev/ttyUSB0"));
      sp->open();
      
      const char* txBuf = "LCD\r";
      uint16_t txBufLen = strlen(txBuf);
      
      vector<uint8_t> txMsg(txBuf, txBuf + txBufLen);
      sp->txData(txMsg);
      
      vector<uint8_t> rxMsg = sp->rxData();
      
      if(rxMsg.size() > 0)
      {
          
          cout << "received " << rxMsg.size() << " bytes: " << string(rxMsg.begin(), rxMsg.end()) << endl;
      }
      
  }
  catch(Exception e)
  {
      LOG_EXCEPTION(e);
  }
  
  sp->close();
  
  int runResult = Fl::run();
  
  LOGI("Goodbye.");
  
  return runResult;
}
