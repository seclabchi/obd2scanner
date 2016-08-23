#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>

#include <cstring>
#include <iostream>
#include <vector>

#include "Logger.h"
#include "Exception.h"

#include "CommInterface.h"

using namespace Tonekids;
using namespace std;

Fl_Window *window = 0;

static int runCommandLineMode();
const char* parseInputString(string inStr);

void exitButtonPressed(Fl_Widget* w, void* v)
{
    if(NULL != window)
    {
        delete window;
    }
    else
    {
        LOGE("Tried to close main window but the pointer is NULL.");
    }
}

int main(int argc, char **argv) 
{
	int no_gui_flag = 0;
	int option_index = 0;
	int c = 0;
	int run_result = 0;
    
    int baud_rate = 0;
    bool baud_rate_set = false;
    string serial_port_device;
    bool serial_port_device_set = false;
    char line_delimiter = '\0';
    bool line_delimiter_set = false;
	
	static struct option long_options[] =
	{
		//these options set a flag
		{"no-gui",    no_argument,       &no_gui_flag, true},
        //non-flag options, distinguished by their indices
        {"port",      required_argument, 0,            'a'},
        {"baudrate",  required_argument, 0,            'b'},
        {"delimiter", required_argument, 0,            'd'},
		{0, 0, 0, 0}
	};
	
	while ((c = getopt_long (argc, argv, "p:b:d:", long_options, &option_index)) != -1)
	{
		switch (c)
		{
			case 0:
			  /* If this option set a flag, do nothing else now. */
			  if (long_options[option_index].flag != 0)
				break;
			  LOGD("option %s", long_options[option_index].name);
			  if (optarg)
				LOGD(" with arg %s", optarg);
			  break;
            case 'p':
              serial_port_device = string(optarg);
              LOGI("Port set to %s", serial_port_device.c_str());
              serial_port_device_set = true;
              break;
            case 'b':
              baud_rate = atoi(optarg);
              LOGI("Baudrate set to %d", baud_rate);
              baud_rate_set = true;
              break;
            case 'd':
              LOGI("Line delimiter set to 0x%02X", optarg[0]);
              line_delimiter = optarg[0];
              line_delimiter_set = true;
              break;
            case ':':
              LOGE("Missing required argument.");
              break;
            case '?':
              LOGE("Detected error with ?");
              break;
			default:
				abort();
		}
	}
	
	if(no_gui_flag)
	{
		LOGI("Running in command line mode.");
	}
	else
	{
		LOGI("Running in GUI mode.");
	}
	
	
  LOGI("Hello and welcome to the OBD2 Scanner.");
  
  if(no_gui_flag)
  {
	  run_result = runCommandLineMode();
  }
  else
  {
	  window = new Fl_Window(320,240);
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
	  
	  Fl_Text_Display* serialLogWin = new Fl_Text_Display(5, 25, 310, 180);
	  Fl_Text_Buffer* serialLogWinBuf = new Fl_Text_Buffer(8192);
	  serialLogWin->buffer(serialLogWinBuf);
	  
	  Fl_Button* exitButton = new Fl_Button(5, 210, 50, 20, "EXIT");
	  exitButton->callback(exitButtonPressed);

	  window->end();
	  window->show(argc, argv);
	  
	  CommInterface* ci = 0;
	  
	  try
	  {
		  ci = new CommInterface(string("/dev/ttyUSB0"), 19200);
		  vector<string> resetResponse = ci->reset();
		  vector<string>::iterator it;
		  
		  for(it = resetResponse.begin(); it != resetResponse.end(); it++)
		  {
			  cout << *it << endl;
		  }
	  }
	  catch(Exception e)
	  {
		  LOG_EXCEPTION(e);
	  }
	  
      if(NULL != ci)
      {
        delete ci;
      }        

          
	  
	  run_result = Fl::run();
  
  }
  
  LOGI("Goodbye.");
  
  return run_result;
}

int runCommandLineMode()
{
    CommInterface* ci = 0;
    string txMsg;
	  
    try
    {
        ci = new CommInterface(string("/dev/ttyUSB0"), 19200);
        
        do
        {
            cin >> txMsg;
            
            if(0 != txMsg.compare(string("quit")))
            {
                const char* requestString = parseInputString(txMsg);
                vector<uint8_t> response = ci->performTransaction(requestString, 250);
                vector<uint8_t>::iterator it;

                for(it = response.begin(); it != response.end(); it++)
                {
                    cout << *it << endl;
                }
            }
        } while(0 != txMsg.compare(string("quit")));
    }
    catch(Exception e)
    {
        LOG_EXCEPTION(e);
    }

    if(NULL != ci)
    {
        delete ci;
    }        

	return 0;
}


const char* parseInputString(string inStr)
{
	uint8_t numArgs = 0;
    char *token = std::strtok((char*)inStr.c_str(), ",");
	
    while (token != NULL) {
        std::cout << token << '\n';
		numArgs++;
        token = std::strtok(NULL, ",");
    }
	
	if(3 != numArgs)
	{
		THROW_EXCEPTION("Wrong number of arguments.");
	}
    
    return inStr.c_str();
}